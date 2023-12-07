/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libgui
\brief Implements the macros that replaces the Qt's signal/slot connect method for cases
when a slot throws exceptions that need to be treated in the limits of the slot execution
preventing the exception to be redirected to the outside world which may lead to
unexpected behavior and crashes.
*/

#ifndef QT_CONNECT_MACROS_H
#define QT_CONNECT_MACROS_H

#include <QObject>

//! \brief This macro expands to a block of code inside a try/catch block*/
#define __trycatch(body) \
try \
{ \
	body \
} \
catch(Exception &e) \
{ \
	Messagebox::error(Exception(e.getErrorMessage(), e.getErrorCode(),\
										__PRETTY_FUNCTION__, __FILE__, __LINE__, &e)); \
}

/*! \brief This macro expands to a QObject::connect call that takes
 * the sender object, the signal emitted by the sender, the receiver object,
 * and the slot method that TAKES NO ARGUMENTS * and the slot method that ONE OR MORE ARGUMENTS
 * hence the "_s0" suffix.
 *
 * The expanded macro encloses the slot in a try/catch block inside a
 * lambda function in order to self-contain all exceptions that eventually go
 * outside the slot scope, avoiding undefined behavior and crashes.
 *
 * NOTE: different from the original Qt's connect function, the slot passed
 * to the macro MUST NOT be the reference (address) of the receiver's method.
 *
 * Usage:
 *
 * > Qt's original
 *		connect(sender, &SenderClass::s_someSignal, receiver, &ReceiverClass::someMethod)
 *
 * > Macro (note the absence of & in slot specification)
 *		__connect_s0(sender, &SenderClass::s_someSignal, receiver, ReceiverClass::someMethod)
 */
#define __connect_s0(sender, signal, receiver, slot, ...) \
connect(sender, signal, receiver, [receiver]() { \
	__trycatch( receiver->slot(); ) \
}, ##__VA_ARGS__);

/*! \brief This macro expands to a QObject::connect call that takes
 * the sender object, the signal emitted by the sender, the receiver object,
 * and the slot method that ONE OR MORE ARGUMENTS hence the "_sn" suffix.
 *
 * The expanded macro encloses the slot in a try/catch block inside a
 * lambda function in order to self-contain all exceptions that eventually go
 * outside the slot scope, avoiding undefined behavior and crashes.
 *
 * NOTE: different from the original Qt's connect function, the slot passed
 * to the macro MUST NOT be the reference (address) of the receiver's method.
 *
 * Usage:
 *
 * > Qt's original
 *		connect(sender, &SenderClass::s_someSignal, receiver, &ReceiverClass::someMethod)
 *
 * > Macro (note the absence of & in slot specification)
 *		__connect_sn(sender, &SenderClass::s_someSignal, receiver, ReceiverClass::someMethod)
 */
#define __connect_sn(sender, signal, receiver, slot, ...) \
connect(sender, signal, receiver, [receiver](auto... sig_args) { \
	__trycatch( receiver->slot(sig_args...); ) \
}, ##__VA_ARGS__);

/*! \brief This macro expands to a QObject::connect call that takes
 * the sender object, the signal emitted by the sender, the context object
 * (where lambda slot is executed), and the lambda slot that TAKES NO ARGUMENTS
 * hence the "_l0" suffix.
 *
 * The expanded macro encloses the lambda slot in a try/catch block inside another
 * lambda function in order to self-contain all exceptions that eventually go
 * outside the lambda slot scope, avoiding undefined behavior and crashes.
 *
 * NOTE: different from the original Qt's connect function, the slot passed
 * to the macro MUST NOT be the reference (address) of the receiver's method.
 *
 * Usage:
 *
 * > Qt's original
 *		connect(sender, &SendeClass::s_someSignal, context, [capture](){ ... })
 *
 * > Macro (note the absence of & in slot specification)
 *		__connect_l0(sender, &SenderClass::s_someSignal, context, [capture](){ ... })
 */
#define __connect_l0(sender, signal, context, lambda, ...) \
{ \
	auto __try_catch_lmbd = [&]() { \
		__trycatch( \
			auto __slot_lmbd = lambda ;\
			__slot_lmbd(); \
		) \
	}; \
	connect(sender, signal, context, __try_catch_lmbd, ##__VA_ARGS__); \
}

/*! \brief This macro expands to a QObject::connect call that takes
 * the sender object, the signal emitted by the sender, the context object
 * (where lambda slot is executed), and the lambda slot that TAKES ONE OR MORE ARGUMENTS
 * hence the "_ln" suffix.
 *
 * The expanded macro encloses the lambda slot in a try/catch block inside another
 * lambda function in order to self-contain all exceptions that eventually go
 * outside the lambda slot scope, avoiding undefined behavior and crashes.
 *
 * NOTE: different from the original Qt's connect function, the slot passed
 * to the macro MUST NOT be the reference (address) of the receiver's method.
 *
 * Usage:
 *
 * > Qt's original
 *		connect(sender, &SendeClass::s_someSignal, context, [capture](){ ... })
 *
 * > Macro (note the absence of & in slot specification)
 *		__connect_ln(sender, &SenderClass::s_someSignal, context, [capture](){ ... })
 */
#define __connect_ln(sender, signal, context, lambda, ...) \
{ \
	auto __try_catch_lmbd = [&](auto... __tc_lmbd_args) { \
		__trycatch( \
			auto __slot_lmbd = lambda ;\
			__slot_lmbd( __tc_lmbd_args...); \
		) \
	}; \
	connect(sender, signal, context, __try_catch_lmbd, ##__VA_ARGS__); \
}

/*! \brief This macro is just a placeholder for the original QObject::connect call.
 * It acts just to flag where a original QObject connect is happening.
 * This macro is for TEMPORARY USE and will be replaced by the original call
 * once the signal/slots connections that need to use try/catch versions are refactored.
 */
#define q_connect(sender, signal, receiver, slot, ...) \
connect(sender, signal, receiver, slot, ##__VA_ARGS__);

#endif
