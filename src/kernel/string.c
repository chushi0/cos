#include <os/string.h>

bool string_starts(const char *str, const char *prefix) {
	while (*str != 0 && *prefix != 0) {
		if (*str != *prefix) {
			return false;
		}
		str++;
		prefix++;
	}
	if (*prefix == 0) {
		return true;
	}
	return false;
}