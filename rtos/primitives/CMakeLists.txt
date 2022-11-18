SET(srcs "mutex.cpp" 
         "queue.cpp"
         "semaphore.cpp")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ".")
