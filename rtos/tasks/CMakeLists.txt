SET(srcs "accelerometer_task.cpp" 
         "receiver_task.cpp"
         "transmitter_task.cpp")
SET(deps primitives
         sensors
         json
         memory)

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS "."
                       REQUIRES ${deps})
