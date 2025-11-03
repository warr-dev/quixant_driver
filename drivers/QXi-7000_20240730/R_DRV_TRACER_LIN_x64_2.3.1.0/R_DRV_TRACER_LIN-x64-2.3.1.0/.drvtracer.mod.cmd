savedcmd_drvtracer.mod := printf '%s\n'   main-tracer.o | awk '!x[$$0]++ { print("./"$$0) }' > drvtracer.mod
