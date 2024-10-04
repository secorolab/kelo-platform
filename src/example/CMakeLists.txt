add_executable(pmu_control
  pmu_control.c
)
target_link_libraries(pmu_control
  ethercat_comm
  kelo_pmu
)

add_executable(mediator_test
  mediator_test.c
)
target_link_libraries(mediator_test
  kelo_mediator
)

add_executable(single_wheel_test
  single_wheel_test.c
)
target_link_libraries(single_wheel_test
  ethercat_comm
  kelo_drive_comm
)

# install in project_name/examples/
install(TARGETS pmu_control mediator_test single_wheel_test
  RUNTIME DESTINATION bin/${PROJECT_NAME}/examples
)