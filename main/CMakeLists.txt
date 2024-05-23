idf_component_register(
        SRCS ./app_main.c
        SRCS Libs/ir_transceiver/ir_transceiver.c
        SRCS Libs/ac_command/ac_command.c
        SRCS Libs/internal_led/internal_led.c

        INCLUDE_DIRS "."
        INCLUDE_DIRS "Libs"
        INCLUDE_DIRS "Libs/ir_transceiver"
        INCLUDE_DIRS "Libs/ac_command"
        INCLUDE_DIRS "Libs/internal_led"
)
