soInjection
===========
soInjection - Linux executable code injection
Copyright (C) 2015  breimer273

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>

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
