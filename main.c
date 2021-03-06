#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

#include "mpc.h"

int main(int argc, char** argv) {

  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expression = mpc_new("expression");
  mpc_parser_t* Lispy = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
            " \
              number: /-?[0-9]+'.'?[0-9]?/ ;                                              \
              operator: '+' | '-' | '+' | '/' | \"add\" | \"div\" | \"mul\" | \"sub\" ; \
              expression: <number> | '(' <operator> <expression>+ ')' ;         \
              lispy: /^/ <operator> <expression>+ /$/ ;                         \
            ", Number, Operator, Expression, Lispy);

  puts("My Lispy Version 0.0.0.1");
  puts("Joseph Bauer");
  puts("Press Ctrl+C to Exit\n");

  // Set up the parsers


  while(1) {

    char* input = readline("my-lisp> ");
    add_history(input);

    //printf("%s\n", input);
    mpc_result_t r;
    if(mpc_parse("<stdin>", input, Lispy, &r)) {
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  mpc_cleanup(4, Number, Operator, Expression, Lispy);

  return 0;
}
