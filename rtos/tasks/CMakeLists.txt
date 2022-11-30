SET(srcs "accelerometer_task.cpp" 
         "receiver_task.cpp"
         "transmitter_task.cpp"
         "display_task.cpp")
SET(deps primitives sensors jsmn memory actuators mqtt network_definitions)

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS "."
                       REQUIRES ${deps})
