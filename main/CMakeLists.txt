SET(srcs "main.cpp"
         "interfaces/i2c.cpp"
         "interfaces/bluetooth_client.cpp")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ".")
