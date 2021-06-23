#pragma once

/**
 * Provides an abstract notion of getting and applying terminal capabilities
 * and settings.  The goal is to make it as simple as possible to set up
 * a terminal which uses UTF-8 and accepts VT emulation, or to know if the
 * terminal does not support both or either of these capabilities.
 *
 * Controllable settings:
 *
 * - UTF-8, to establish a common encoding, since its use and support is widespread
 * - Echo, for private data
 * - Line input, allowing for incrementally handling input
 * - VT escape sequences, for controlling terminal display and colors.
 */
namespace terminal {

/**
 * Some settings apply to the terminal as a whole, and not to individual
 * IO streams.  These should be set on startup and setup might fail.
 */
bool init();

/**
 * Some settings persist after program termination, such as changing the code
 * page on Windows.  Thus a way is needed to restore settings to their original
 * values.
 */
void shutdown();

#ifndef NDEBUG
/**
 * Prints the capabilities of the terminal in an implementation-specific way
 * for debugging purposes.
 */
void printCapabilities();
#endif // NDEBUG

enum class InputSetting {
    Echo,
    LineInput
};

enum class OutputSetting {
    EscapeSequences
};

void set(InputSetting in, bool enabled);
void set(OutputSetting out, bool enabled);

} // namespace console
