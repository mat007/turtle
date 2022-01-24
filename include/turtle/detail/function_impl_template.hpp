// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "expectation_template.hpp"

#ifndef MOCK_ERROR_POLICY
#    error no error policy has been set
#endif

#define MOCK_FUNCTION_FORMAT(z, n, N) << ' ' << mock::format(t##n) << BOOST_PP_IF(BOOST_PP_EQUAL(N, n), ' ', ',')

#define MOCK_FUNCTION_CONTEXT                                                                                        \
    boost::unit_test::lazy_ostream::instance()                                                                       \
      << lazy_context(this) << '(' BOOST_PP_REPEAT(MOCK_NUM_ARGS, MOCK_FUNCTION_FORMAT, BOOST_PP_DEC(MOCK_NUM_ARGS)) \
      << ')' << lazy_expectations(this)

#define MOCK_FORWARD(z, n, d) std::forward<T##n>(t##n)

namespace mock { namespace detail {
    template<typename Signature>
    class function_impl;

    template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(MOCK_NUM_ARGS, typename T)>
    class function_impl<R(BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, T))> :
        public verifiable,
        public std::enable_shared_from_this<function_impl<R(BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, T))>>
    {
    public:
        typedef safe_error<R, MOCK_ERROR_POLICY<R>> error_type;

    public:
        function_impl() : context_(0), valid_(true), exceptions_(exceptions()), mutex_(std::make_shared<mutex>()) {}
        virtual ~function_impl()
        {
            if(valid_ && exceptions_ >= exceptions())
            {
                for(const auto& expectation : expectations_)
                {
                    if(!expectation.verify())
                    {
                        error_type::fail("untriggered expectation",
                                         boost::unit_test::lazy_ostream::instance()
                                           << lazy_context(this) << lazy_expectations(this),
                                         expectation.file(),
                                         expectation.line());
                    }
                }
            }
            if(context_)
                context_->remove(*this);
        }

        virtual bool verify() const
        {
            lock _(mutex_);
            for(const auto& expectation : expectations_)
            {
                if(!expectation.verify())
                {
                    valid_ = false;
                    error_type::fail("verification failed",
                                     boost::unit_test::lazy_ostream::instance()
                                       << lazy_context(this) << lazy_expectations(this),
                                     expectation.file(),
                                     expectation.line());
                }
            }
            return valid_;
        }

        virtual void reset()
        {
            lock _(mutex_);
            valid_ = true;
            std::shared_ptr<function_impl> guard = this->shared_from_this();
            expectations_.clear();
        }

    private:
        typedef expectation<R(BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, T))> expectation_type;

        class wrapper : public wrapper_base<R, expectation_type>
        {
        private:
            typedef wrapper_base<R, expectation_type> base_type;

        public:
            wrapper(const std::shared_ptr<mutex>& m, expectation_type& e) : base_type(e), lock_(m) {}
            wrapper(const wrapper&) = delete;
            wrapper(wrapper&& x) = default;
            wrapper& operator=(const wrapper&) = delete;
            wrapper& operator=(wrapper&& x) = default;
            wrapper& once()
            {
                this->e_->invoke(std::make_unique<detail::once>());
                return *this;
            }
            wrapper& never()
            {
                this->e_->invoke(std::make_unique<detail::never>());
                return *this;
            }
            wrapper& exactly(std::size_t count)
            {
                this->e_->invoke(std::make_unique<detail::exactly>(count));
                return *this;
            }
            wrapper& at_least(std::size_t min)
            {
                this->e_->invoke(std::make_unique<detail::at_least>(min));
                return *this;
            }
            wrapper& at_most(std::size_t max)
            {
                this->e_->invoke(std::make_unique<detail::at_most>(max));
                return *this;
            }
            wrapper& between(std::size_t min, std::size_t max)
            {
                this->e_->invoke(std::make_unique<detail::between>(min, max));
                return *this;
            }

#ifndef MOCK_NUM_ARGS_0
            template<BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, typename Constraint_)>
            wrapper& with(BOOST_PP_ENUM_BINARY_PARAMS(MOCK_NUM_ARGS, Constraint_, c))
            {
                this->e_->with(BOOST_PP_ENUM_PARAMS(MOCK_NUM_ARGS, c));
                return *this;
            }

#    if MOCK_NUM_ARGS > 1
            template<typename Constraint>
            wrapper& with(const Constraint& c)
            {
                this->e_->with(c);
                return *this;
            }
#    endif
#endif

#define MOCK_FUNCTION_IN_ADD(z, n, d) this->e_->add(s##n);

#define MOCK_FUNCTION_IN(z, n, d)                     \
    wrapper& in(BOOST_PP_ENUM_PARAMS(n, sequence& s)) \
    {                                                 \
        BOOST_PP_REPEAT(n, MOCK_FUNCTION_IN_ADD, _)   \
        return *this;                                 \
    }

            BOOST_PP_REPEAT(MOCK_MAX_SEQUENCES, MOCK_FUNCTION_IN, _)

#undef MOCK_FUNCTION_IN
#undef MOCK_FUNCTION_IN_ADD

            template<typename TT>
            void calls(TT t)
            {
                this->e_->calls(t);
            }
            template<typename TT>
            void throws(TT t)
            {
                this->e_->throws(t);
            }
            template<typename TT>
            void moves(TT&& t)
            {
                this->e_->moves(std::move(t));
            }

            lock lock_;
        };

    public:
        typedef wrapper wrapper_type;

        wrapper expect(const char* file, int line)
        {
            lock _(mutex_);
            expectations_.emplace_back(file, line);
            valid_ = true;
            return wrapper(mutex_, expectations_.back());
        }
        wrapper expect()
        {
            lock _(mutex_);
            expectations_.emplace_back();
            valid_ = true;
            return wrapper(mutex_, expectations_.back());
        }

        R operator()(BOOST_PP_ENUM_BINARY_PARAMS(MOCK_NUM_ARGS, T, t)) const
        {
            lock _(mutex_);
            valid_ = false;
            for(const auto& expectation : expectations_)
            {
                if(expectation.is_valid(BOOST_PP_ENUM(MOCK_NUM_ARGS, MOCK_FORWARD, _)))
                {
                    if(!expectation.invoke())
                    {
                        error_type::fail(
                          "sequence failed", MOCK_FUNCTION_CONTEXT, expectation.file(), expectation.line());
                        return error_type::abort();
                    }
                    if(!expectation.valid())
                    {
                        error_type::fail(
                          "missing action", MOCK_FUNCTION_CONTEXT, expectation.file(), expectation.line());
                        return error_type::abort();
                    }
                    valid_ = true;
                    error_type::call(MOCK_FUNCTION_CONTEXT, expectation.file(), expectation.line());
                    if(expectation.functor())
                        return expectation.functor()(BOOST_PP_ENUM(MOCK_NUM_ARGS, MOCK_FORWARD, _));
                    return expectation.trigger();
                }
            }
            error_type::fail("unexpected call", MOCK_FUNCTION_CONTEXT);
            return error_type::abort();
        }

        void add(context& c,
                 const void* p,
                 boost::unit_test::const_string instance,
                 boost::optional<type_name> type,
                 boost::unit_test::const_string name)
        {
            lock _(mutex_);
            if(!context_)
                c.add(*this);
            c.add(p, *this, instance, type, name);
            context_ = &c;
        }

        friend std::ostream& operator<<(std::ostream& s, const function_impl& impl)
        {
            lock _(impl.mutex_);
            return s << lazy_context(&impl) << lazy_expectations(&impl);
        }

        struct lazy_context
        {
            lazy_context(const function_impl* impl) : impl_(impl) {}
            friend std::ostream& operator<<(std::ostream& s, const lazy_context& c)
            {
                if(c.impl_->context_)
                    c.impl_->context_->serialize(s, *c.impl_);
                else
                    s << '?';
                return s;
            }
            const function_impl* impl_;
        };

        struct lazy_expectations
        {
            lazy_expectations(const function_impl* impl) : impl_(impl) {}
            friend std::ostream& operator<<(std::ostream& s, const lazy_expectations& e)
            {
                for(const auto& expectation : e.impl_->expectations_)
                    s << std::endl << expectation;
                return s;
            }
            const function_impl* impl_;
        };

        std::list<expectation_type> expectations_;
        context* context_;
        mutable bool valid_;
        const int exceptions_;
        const std::shared_ptr<mutex> mutex_;
    };
}} // namespace mock::detail

#undef MOCK_FUNCTION_FORMAT
#undef MOCK_FUNCTION_CONTEXT
#undef MOCK_FORWARD
