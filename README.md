# profile_tracer
Profiles the mutex locking overhead of the Jaeger C++ client library Tracer.  

# Cloning
Clone the repository using `git clone --recursive git@github.com:CppPhil/profile_tracer.git`  

# Building
Build in debug mode without Jaeger patch: `./build.sh --build_type=Debug --apply_patch=false`  
Build in release mode without Jaeger patch: `./build.sh --build_type=Release --apply_patch=false`  
Build in debug mode with Jaeger patch: `./build.sh --build_type=Debug --apply_patch=true`  
Build in release mode with Jaeger patch `./build.sh --build_type=Release --apply_patch=true`  

# Running
After having built the project the application can be run using `./run.sh`  
