idf_component_register(
        SRCS ./app_main.c
        SRCS ./lightbulb.c
        SRCS Libs/ir_transceiver/ir_transceiver.c
        SRCS Libs/ac_command/ac_command.c

        INCLUDE_DIRS "."
        INCLUDE_DIRS "Libs"
        INCLUDE_DIRS "Libs/ir_transceiver"
        INCLUDE_DIRS "Libs/ac_command"
)
