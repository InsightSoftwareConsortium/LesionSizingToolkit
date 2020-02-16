# The PARSE_ARGUMENTS macro, defined below, can be used by other macros to 
# parse arguments defined in this way.
#
# The PARSE_ARGUMENTS macro will take the arguments of another macro and define 
# several variables. The first argument to PARSE_ARGUMENTS is a prefix to 
# put on all variables it creates. The second argument is a list of names, 
# and the third argument is a list of options. Both of these lists should be 
# quoted. The rest of PARSE_ARGUMENTS are arguments from another macro to be parsed.
#
#    PARSE_ARGUMENTS(prefix arg_names options arg1 arg2...) 
#
# For each item in options, PARSE_ARGUMENTS will create a variable with that name, 
# prefixed with prefix_. So, for example, if prefix is MY_MACRO and options is 
# OPTION1;OPTION2, then PARSE_ARGUMENTS will create the variables 
# MY_MACRO_OPTION1 and MY_MACRO_OPTION2. These variables will be set to true if 
# the option exists in the command line or false otherwise.
#
# For each item in arg_names, PARSE_ARGUMENTS will create a variable with that name,
# prefixed with prefix_. Each variable will be filled with the arguments that occur after 
# the given arg_name is encountered up to the next arg_name or the end of the arguments. 
# All options are removed from these lists. PARSE_ARGUMENTS also creates a 
# prefix_DEFAULT_ARGS variable containing the list of all arguments up to the first
# arg_name encountered.
#
# Here is a simple, albeit impractical, example of using PARSE_ARGUMENTS that demonstrates its behavior.
#
# set(arguments
#   hello OPTION3 world
#   LIST3 foo bar
#   OPTION2
#   LIST1 fuz baz
#   )
# PARSE_ARGUMENTS(ARG "LIST1;LIST2;LIST3" "OPTION1;OPTION2;OPTION3" ${arguments})
# 
# PARSE_ARGUMENTS creates 7 variables and sets them as follows:
# 
#     * ARG_DEFAULT_ARGS: hello;world
#     * ARG_LIST1: fuz;baz
#     * ARG_LIST2:
#     * ARG_LIST3: foo;bar
#     * ARG_OPTION1: FALSE
#     * ARG_OPTION2: TRUE
#     * ARG_OPTION3: TRUE 
# 
# If you don't have any options, use an empty string in its place.
# 
# PARSE_ARGUMENTS(ARG "LIST1;LIST2;LIST3" "" ${arguments})
# 
# Likewise if you have no lists.
# 
# PARSE_ARGUMENTS(ARG "" "OPTION1;OPTION2;OPTION3" ${arguments})
# 
# 
macro(PARSE_ARGUMENTS prefix arg_names option_names)
  set(DEFAULT_ARGS)
  foreach(arg_name ${arg_names})    
    set(${prefix}_${arg_name})
  endforeach(arg_name)
  foreach(option ${option_names})
    set(${prefix}_${option} FALSE)
  endforeach(option)

  set(current_arg_name DEFAULT_ARGS)
  set(current_arg_list)
  foreach(arg ${ARGN})            
    set(larg_names ${arg_names})    
    list(FIND larg_names "${arg}" is_arg_name)                   
    if (is_arg_name GREATER -1)
      set(${prefix}_${current_arg_name} ${current_arg_list})
      set(current_arg_name ${arg})
      set(current_arg_list)
    else (is_arg_name GREATER -1)
      set(loption_names ${option_names})    
      list(FIND loption_names "${arg}" is_option)            
      if (is_option GREATER -1)
	     set(${prefix}_${arg} TRUE)
      else (is_option GREATER -1)
	     set(current_arg_list ${current_arg_list} ${arg})
      endif (is_option GREATER -1)
    endif (is_arg_name GREATER -1)
  endforeach(arg)
  set(${prefix}_${current_arg_name} ${current_arg_list})
endmacro(PARSE_ARGUMENTS)
