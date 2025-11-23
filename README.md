# ministl

__ministl__ provides a small framework for projects which cannot (or would not) use the standard c++ library. 
It does not use exceptions and contains only a very limited set of capabilities, including
replacements for _libc_/_os_ provided capabilities (like _heap_, _mutex_ and more).

## Usage

### Required Primitives

When using, each project must provide several primitives implemented specifically for the environment
on which it runs, these are:
- `framework::terminate`: this function must terminate the program and not return. Can be implemented by rebooting the system,
    halting the cpu or more. This function should not use any __ministl__ code as it is a global dependency for much of it.
- `framework::debug::trace_impl`: this function should output debug traces, equivalent for writing into _stdout_ in normal
    _os_. One example implementation may write on the screen, or to a file. This function should not use any __ministl__ code as it is a global dependency for much of it.
- `framework::heap::*`: these are the heap allocation functions. One should implement them with the help of the `framework::heap::heap`
    class. These functions should not use most of __ministl__ code as it is a global dependency for some of it. The functions are
  - `framework::heap::malloc`
  - `framework::heap::realloc`
  - `framework::heap::calloc`
  - `framework::heap::free`

Your code will not compile without these functions implemented. But how to implement very much depends on the environment 
you are running on.

### Controlling Included Features

It is possible to control certain features via defines passed to cmake
- `MINISTL_DEFINE_TYPES`: if set, __ministl__ will include typedefs for standard types, like `uint64_t` and such.
- `MINISTL_TYPES_FROM_STD`: if set, __ministl__ will include typedefs from `stdint.h`.
- `MINISTL_SIZET_SIZE`: if set, __ministl__ will define `size_t` and `ssize_t`. The value set (1 or 2) will define the size given to these types.
- `MINISTL_DEFINE_CRT`: if set, __ministl__ will define `new` and `delete` operator overloads to use its own heap.
- `MINISTL_DEFINE_INTRINSIC`: iif set, __ministl__ will define common intrinsics like `memset` and `memcpy`.

### Status Codes

Because __ministl__ does not use exceptions, all functions that may fail work with error codes. `status` defines the base
type containing the error code. Each error code is a simple `uint64_t` accompanied by a `uint8_t` category which defines whose
code it is. This allows reuse of the same codes from different sources.

Most functions will actually return a `result` type. Which includes either a value or a `status` error. This allows
returning values from the functions.

The following function uses `result`:
```c++
framework::result<int> do_something() {
    if (error_cause) {
        return framework::err({category, status_code});
    }
    
    return framework::ok(value);
}
```

To work with functions and `result` you can use the `verify.h` header which includes macros
to help handle such functions with ease. Example:
```c++
result<> some() {
    // if the function returned a failure result, the current function will return with a result error, propegating it
    // otherwise value variable will receive the value from the result. 
    auto value = verify(function_with_result());
    // do something .....
    return {}; // return success
}
```
