#ifndef UTILS_H
#define UTILS_H


#define UNUSED(arg) (void)arg
#define UNREACHABLE(message) do {                       \
	printf("UNREACHABLE: " message "\n"); abort();  \
} while (0)

#endif // UTILS_H
