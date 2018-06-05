#define ERR(EVAL, ...) fmt.Fprintf(os.Stderr, __VA_ARGS__); os.Exit(EVAL)
#define WARN(...)      fmt.Fprintf(os.Stderr, __VA_ARGS__)
