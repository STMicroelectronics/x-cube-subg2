#ifndef __COMMAND_INTERPETRER_H__
#define __COMMAND_INTERPETRER_H__
/** @file command-interpreter2.h
 * @brief Processes commands coming from the serial port.
 ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
 ******************************************************************************
*/
#include <stdint.h>

#ifndef MAX_COMMAND_ARGUMENTS
/** The maximum number of arguments a command can have.  A nested command
 * counts as an argument.
 */
#define MAX_COMMAND_ARGUMENTS 16
#endif

#ifndef COMMAND_BUFFER_LENGTH
#define COMMAND_BUFFER_LENGTH 1024
#endif

// The (+ 1) takes into account the leading command.
#define MAX_TOKEN_COUNT (MAX_COMMAND_ARGUMENTS + 1)

typedef void (*CommandAction)(void);

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**@brief Command entry for a command table.
 */
typedef struct {
#else
typedef struct {
#endif
  /** Use letters, digits, and underscores, '_', for the command name.
   * Command names are case-sensitive.
   */
  const char * name;
  /** A reference to a function in the application that implements the
   *  command.
   */
  CommandAction action;
  /** String that specifies the number and types of arguments the
   * command accepts.  The argument specifiers are:
   *  - u:   one-byte unsigned integer.
   *  - v:   two-byte unsigned integer
   *  - w:   four-byte unsigned integer
   *  - s:   one-byte signed integer
   *  - b:   string.  The argument can be entered in ascii by using
   *         quotes, for example: "foo".  Or it may be entered
   *         in hex by using curly braces, for example: { 08 A1 f2 }.
   *         There must be an even number of hex digits, and spaces
   *         are ignored.
   *  - n:   indicates this is a 'n'ested command.
   *         The action points to a table of subcommands.
   *         If used, this must be the only specifier.
   *         It also adds one to the total argument count of the
   *         complete command.
   *
   *  Integer arguments can be either decimal or hexidecimal.
   *  A 0x prefix indicates a hexidecimal integer.  Example: 0x3ed.
   */
  const char * argumentTypes;
  /** A description of the command.
   */
  const char * description;
} CommandEntry;

extern CommandEntry CommandTable[];

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/** @brief Command error states.
 *
 * If you change this list, ensure you also change the strings that describe
 * these errors in the array CommandErrorNames[] in command-interpreter.c.
 */
enum CommandStatus
#else
typedef uint8_t CommandStatus;
enum
#endif
{
  CMD_SUCCESS,
  CMD_ERR_PORT_PROBLEM,
  CMD_ERR_NO_SUCH_COMMAND,
  CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS,
  CMD_ERR_ARGUMENT_OUT_OF_RANGE,
  CMD_ERR_ARGUMENT_SYNTAX_ERROR,
  CMD_ERR_STRING_TOO_LONG,
  CMD_ERR_INVALID_ARGUMENT_TYPE
};

/** @name Functions to Retrieve Arguments
 * Use the following functions in your functions that process commands to
 * retrieve arguments from the command interpreter.
 * These functions pull out unsigned integers, signed integers, and strings,
 * and hex strings.  Index 0 is the first command argument.
 *@{
 */

/** Retrieves unsigned integer arguments. */
uint32_t unsignedCommandArgument(uint8_t index);

/** Retrieves signed integer arguments. */
int16_t signedCommandArgument(uint8_t index);

/** Retrieve quoted string or hex string arguments.
 * Hex strings have already been converted into binary.
 * To retrieve the name of the command itself, use an index of -1.
 * For example, to retrieve the first character of the command, do:
 * uint8_t firstChar = stringCommandArgument(-1, NULL)[0].
 * If the command is nested, an index of -2, -3, etc will work to retrieve
 * the higher level command names.
 */
uint8_t *stringCommandArgument(int8_t index, uint8_t *length);

/** Copies the string argument to the given destination up to maxLength.
 * If the argument length is nonzero but less than maxLength
 * and leftPad is TRUE, leading zeroes are prepended to bring the
 * total length of the target up to maxLength.  If the argument
 * is longer than the maxLength, it is truncated to maxLength.
 * Returns the minimum of the argument length and maxLength.
 *
 * This function is commonly used for reading in hex strings
 * such as EUI64 or key data and left padding them with zeroes.
 * See ::CopyKeyArgument and ::CopyEui64Argument for
 * convenience macros for this purpose.
 */
uint8_t copyStringArgument(int8_t index,
                              uint8_t *destination,
                              uint8_t maxLength,
                              uint8_t leftPad);

#if 0 // ????
/** A convenience macro for copying security key arguments to an
 * KeyData pointer.
 */
#define CopyKeyArgument(index, keyDataPointer)            \
  (CopyStringArgument((index),                            \
                           KeyContents((keyDataPointer)), \
                           ENCRYPTION_KEY_SIZE,          \
                           TRUE))
#endif
/** A convenience macro for copying eui64 arguments to an EUI64. */
#define copyEui64Argument(index, eui64) \
  (CopyStringArgument((index), (eui64), EUI64_SIZE, TRUE))

/** @} // END name group
 */

/** The application may implement this handler.  To override
 * the default handler, define APPLICATION_HAS_COMMAND_ERROR_HANDLER
 * in the CONFIGURATION_HEADER.
 */
void commandErrorHandler(CommandStatus status);

/** @brief Initialize the command interpreter.
 */
void commandReaderInit(void);

/** @brief Process the given string as a command.
 */
uint8_t processCommandString(uint8_t *input, uint8_t size);

/** @brief Process input coming in on the given serial port.
 * @return TRUE if an end of line character was read.
 * If the application uses a command line prompt,
 * this indicates it is time to print the prompt.
 * @code
 * void processCommandInput(uint8_t port);
 * @endcode
 */
#define processCommandInput() processCommandString(NULL,0)

CommandEntry* getCurrentCommandFinger(void);

/** @} // END addtogroup
*/
#endif /* __COMMAND_INTERPETRER_H__ */
