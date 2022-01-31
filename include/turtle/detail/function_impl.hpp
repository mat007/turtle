// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_FUNCTION_IMPL_HPP_INCLUDED
#define MOCK_FUNCTION_IMPL_HPP_INCLUDED

#include "../error.hpp"
#include "expectation.hpp"
#include "mutex.hpp"
#include "verifiable.hpp"
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/test/utils/lazy_ostream.hpp>
#include <list>
#include <memory>

#ifndef MOCK_ERROR_POLICY
#    error no error policy has been set
#endif

namespace mock { namespace detail {
    template<typename R, typename E>
    struct wrapper_base
    {
        wrapper_base(E& e) : e_(&e) {}

        template<typename T>
        void returns(T t)
        {
            e_->returns(t);
        }

        E* e_;
    };
    template<typename E>
    struct wrapper_base<void, E>
    {
        wrapper_base(E& e) : e_(&e) {}

        E* e_;
    };
    template<typename R, typename E>
    struct wrapper_base<R*, E>
    {
        wrapper_base(E& e) : e_(&e) {}

        void returns(R* r) { e_->returns(r); }
        template<typename Y>
        void returns(const std::reference_wrapper<Y>& r)
        {
            e_->returns(r);
        }

        E* e_;
    };

    inline int exceptions()
    {
#ifdef MOCK_UNCAUGHT_EXCEPTIONS
        using namespace std;
        return uncaught_exceptions();
#else
        return std::uncaught_exception() ? 1 : 0;
#endif
    }

    template<typename Signature>
    class function_impl;

    template<typename R, typename... Args>
    class function_impl<R(Args...)> : public verifiable, public std::enable_shared_from_this<function_impl<R(Args...)>>
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
        typedef expectation<R(Args...)> expectation_type;

        class wrapper : public wrapper_base<R, expectation_type>
        {
        private:
            typedef wrapper_base<R, expectation_type> base_type;
            static constexpr auto arity = sizeof...(Args);

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

            /// Callable only for functions taking arguments
            /// Number of constraints must match the number of arguments
            /// or a single constraint checking all arguments must be passed
            template<typename... Constraints>
            std::enable_if_t<(arity > 0u && (sizeof...(Constraints) == arity || sizeof...(Constraints) == 1u)),
                             wrapper&>
            with(Constraints... c)
            {
                this->e_->with(c...);
                return *this;
            }

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

        R operator()(Args... args) const
        {
// Due to lifetime rules of references this must be created and consumed in one line
#define MOCK_FUNCTION_CONTEXT \
    boost::unit_test::lazy_ostream::instance() << lazy_context(this) << lazy_args(args...) << lazy_expectations(this)

            lock _(mutex_);
            valid_ = false;
            for(const auto& expectation : expectations_)
            {
                if(expectation.is_valid(std::forward<Args>(args)...))
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
                        return expectation.functor()(std::forward<Args>(args)...);
                    return expectation.trigger();
                }
            }
            error_type::fail("unexpected call", MOCK_FUNCTION_CONTEXT);
            return error_type::abort();
#undef MOCK_FUNCTION_CONTEXT
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

        struct lazy_args
        {
            lazy_args(std::add_lvalue_reference_t<Args>... args) : args_(args...) {}
            friend std::ostream& operator<<(std::ostream& s, const lazy_args& a)
            {
                return a.print_impl(std::make_index_sequence<sizeof...(Args)>{}, s);
            }

        private:
            std::tuple<std::add_lvalue_reference_t<Args>...> args_;

            template<std::size_t... I>
            std::ostream& print_impl(std::index_sequence<I...>, std::ostream& s) const
            {
                s << '(';
                using expander = int[];
                (void)expander{
                    0, (s << ' ' << mock::format(std::get<I>(args_)) << (sizeof...(Args) - 1u == I ? ' ' : ','), 0)...
                };
                return s << ')';
            }
        };

        std::list<expectation_type> expectations_;
        context* context_;
        mutable bool valid_;
        const int exceptions_;
        const std::shared_ptr<mutex> mutex_;
    };
}} // namespace mock::detail

#endif // MOCK_FUNCTION_IMPL_HPP_INCLUDED
