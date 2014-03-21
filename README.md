soInjection
===========

Like DLL injection but for linux libraries.

IDEA:
I want to replicate DLL injection but I want to do it in linux. So far all of my google
searches have returned nothing about how to do this the way I want to do it.

Google mostly says that you have to change an environment variable before executing
the application so that the library you are trying to inject gets loaded before every
other library. Not really what I am trying to accomplish. I am trying to inject
a library (.so file) into an already running process in linux. Then possibly gain execution
by patching some system functions and things.

Refer to the PROBLEMS file to take a look at the problems that I am currently having, or
foresee having in the near future. Please feel free to provide suggestions.
