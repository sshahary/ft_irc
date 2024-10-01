/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musenov <musenov@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:33:12 by musenov           #+#    #+#             */
/*   Updated: 2024/10/01 22:51:30 by musenov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcException.hpp"

IrcException::IrcException(const std::string &e_message) : 
												exception_message(e_message)
{
}

/**
 * Empty throw(): When a function, in this case the destructor, is declared 
 * with throw(), it means the function promises not to throw any exceptions. 
 * It's equivalent to saying "this function throws nothing".
 * 
 * Destructors are special because they are called during stack unwinding when 
 * an exception is being handled. If a destructor were to throw an exception 
 * while another exception was already being handled, it would lead to 
 * undefined behavior. Therefore, it's generally considered good practice to 
 * ensure destructors don't throw exceptions.
 */

IrcException::~IrcException() throw()
// virtual ~IrcException() noexcept; // in C++11 and later
{
}

const char *IrcException::what() const throw()
{
	return exception_message.c_str();
}

/**
 * The empty `throw()` specification in this context serves a specific purpose.
 *  Let's break it down:

1. Override of std::exception::what():
   The `what()` function is overriding the virtual function from the base 
   class `std::exception`.

2. Exception Specification Matching:
   In C++98, when overriding a virtual function, the exception specification 
   of the overriding function must be at least as strict as the base class 
   function it's overriding.

3. std::exception::what() specification:
   The `what()` function in `std::exception` is declared as:
   
   `virtual const char* what() const throw();`
   
   It uses the empty `throw()` specification, meaning it promises not to throw 
   any exceptions.

4. Compliance requirement:
   To properly override this function, your `IrcException::what()` must also 
   promise not to throw any exceptions. Hence, it also uses `throw()`.

 */
