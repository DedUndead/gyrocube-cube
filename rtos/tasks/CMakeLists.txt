SET(srcs "accelerometer_task.cpp" 
         "receiver_task.cpp"
         "transmitter_task.cpp")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS "."
                       REQUIRES primitives
                                sensors)
