# C-Thread-Template
A simple template that can be used to create a threaded C++ application on a single machine (ie. All the threads are run on one PC, not over the network).

This uses the C++ Standard Future library. It allows you to call functions asyncronously and then waits for the result when you call the `.get()` function.
