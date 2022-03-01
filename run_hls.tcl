open_project -reset linear_proj

add_files model.h
add_files model.cc
# add_files main_tb.ccless -

add_files -tb main_tb.cc

set_top linear_model

open_solution "solution1" -flow_target vivado
set_part xcu50-fsvh2104-2-e
create_clock -period 3.33 -name default

# csim_design
csynth_design
# cosim_design -O -enable_dataflow_profiling -trace_level all