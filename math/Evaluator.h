/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MATH_EVALUATOR_H
#define TOS_MATH_EVALUATOR_H

#include "../stdlib/Types.h"
#include "../utils/StringUtils.h"
#include <stdio.h>
#include <stdlib.h>

#define EVALUATOR_MAX_STACK_SIZE 256

// Stack for operators
struct EvaluatorOperatorStack {
    char items[EVALUATOR_MAX_STACK_SIZE];
    int32 top;
};

// Stack for values
struct EvaluatorValueStack {
    f32 items[EVALUATOR_MAX_STACK_SIZE];
    int32 top;
};

struct EvaluatorVariable {
    char name[32];
    f32 value;
};

static inline
const char* evaluator_skip_whitespace(const char* str) {
    while (*str && (*str == ' ' || *str == '\t')) {
        ++str;
    }

    return str;
}

// Stack operations
void evaluator_push_operator(EvaluatorOperatorStack* stack, char op) {
    if (stack->top >= EVALUATOR_MAX_STACK_SIZE - 1) {
        return;
    }

    stack->items[++stack->top] = op;
}

char evaluator_pop_operator(EvaluatorOperatorStack* stack) {
    ASSERT_SIMPLE(stack->top >= 0);

    return stack->items[stack->top--];
}

char evaluator_peek_operator(EvaluatorOperatorStack* stack) {
    if (stack->top < 0) {
        return '\0';
    }

    return stack->items[stack->top];
}

void evaluator_push_value(EvaluatorValueStack* stack, f32 value) {
    if (stack->top >= EVALUATOR_MAX_STACK_SIZE - 1) {
        return;
    }

    stack->items[++stack->top] = value;
}

f32 evaluator_pop_value(EvaluatorValueStack* stack) {
    ASSERT_SIMPLE(stack->top >= 0);

    return stack->items[stack->top--];
}

// Precedence of operators
int32 evaluator_precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

// Apply an operator to two values
f32 evaluator_apply_operator(char op, f32 a, f32 b) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default: {
            UNREACHABLE();
        }
    }
}

f32 evaluator_evaluate_function(const char* name, const char* args);

// Shunting-yard algorithm to evaluate the expression
f32 evaluator_evaluate_expression(const char* expr) {
    EvaluatorOperatorStack operators = { .top = -1 };
    EvaluatorValueStack values = { .top = -1 };

    const char* ptr = expr;
    while (*ptr) {
        ptr = evaluator_skip_whitespace(ptr);

        if (str_is_num(*ptr) || *ptr == '.') {
            // Parse number
            char* end;
            f32 value = strtof(ptr, &end);
            evaluator_push_value(&values, value);
            ptr = end;
        } else if (str_is_alpha(*ptr)) {
            // Parse function
            const char* start = ptr;
            while (str_is_alphanum(*ptr)) {
                ++ptr;
            }

            ASSERT_SIMPLE(*ptr == '(');

            ++ptr;
            const char* args_start = ptr;
            int32 depth = 1;
            while (*ptr && depth > 0) {
                if (*ptr == '(') {
                    ++depth;
                } else if (*ptr == ')') {
                    --depth;
                }

                ++ptr;
            }

            ASSERT_SIMPLE(depth == 0);

            char name[32];
            memcpy(name, start, ptr - start);
            name[ptr - start] = '\0';

            char args[128];
            memcpy(args, args_start, ptr - args_start - 1);
            args[ptr - args_start - 1] = '\0';

            f32 result = evaluator_evaluate_function(name, args);
            evaluator_push_value(&values, result);
        } else if (*ptr == '(') {
            // Open parenthesis
            evaluator_push_operator(&operators, *ptr);
            ++ptr;
        } else if (*ptr == ')') {
            // Close parenthesis
            while (evaluator_peek_operator(&operators) != '(') {
                char op = evaluator_pop_operator(&operators);
                f32 b = evaluator_pop_value(&values);
                f32 a = evaluator_pop_value(&values);
                evaluator_push_value(&values, evaluator_apply_operator(op, a, b));
            }

            evaluator_pop_operator(&operators); // Remove '('
            ++ptr;
        } else if (strchr("+-*/", *ptr)) {
            // Operator
            char op = *ptr;
            while (evaluator_precedence(evaluator_peek_operator(&operators)) >= evaluator_precedence(op)) {
                char top_op = evaluator_pop_operator(&operators);
                f32 b = evaluator_pop_value(&values);
                f32 a = evaluator_pop_value(&values);
                evaluator_push_value(&values, evaluator_apply_operator(top_op, a, b));
            }

            evaluator_push_operator(&operators, op);
            ++ptr;
        } else {
            UNREACHABLE();
        }
    }

    // Process remaining operators
    while (operators.top >= 0) {
        char op = evaluator_pop_operator(&operators);
        f32 b = evaluator_pop_value(&values);
        f32 a = evaluator_pop_value(&values);
        evaluator_push_value(&values, evaluator_apply_operator(op, a, b));
    }

    return evaluator_pop_value(&values);
}

// Evaluate built-in functions
f32 evaluator_evaluate_function(const char* name, const char* args) {
    if (str_compare(name, "min") == 0) {
        const char* comma = strchr(args, ',');
        if (!comma) {
            return 0.0;
        }

        char arg1[64], arg2[64];
        memcpy(arg1, args, comma - args);
        arg1[comma - args] = '\0';
        str_copy_short(arg2, comma + 1);

        f32 val1 = evaluator_evaluate_expression(arg1);
        f32 val2 = evaluator_evaluate_expression(arg2);

        return OMS_MIN(val1, val2);
    } else if (str_compare(name, "max") == 0) {
        const char* comma = strchr(args, ',');
        if (!comma) {
            return 0.0;
        }

        char arg1[64], arg2[64];
        memcpy(arg1, args, comma - args);
        arg1[comma - args] = '\0';
        str_copy_short(arg2, comma + 1);

        f32 val1 = evaluator_evaluate_expression(arg1);
        f32 val2 = evaluator_evaluate_expression(arg2);

        return OMS_MAX(val1, val2);
    }

    return 0.0;
}

f32 evaluator_evaluate(char* expr, int32 variable_count = 0, const EvaluatorVariable* variables = NULL) {
    // Handle variables
    const char* ptr = expr;
    int32 available_variables = variable_count;

    while (*ptr && available_variables) {
        // Skip none-alpha values
        while (!str_is_alpha(*ptr) && *ptr != '\0') {
            ++ptr;
        }

        if (*ptr == '\0') {
            continue;
        }

        // Potential variable name
        for (int32 i = 0; i < variable_count; ++i) {
            size_t len = strlen(variables[i].name);

            // Check  if string is variable (must be followed by a whitespace or end of string)
            if (str_compare(ptr, variables[i].name, len) == 0 && (ptr[len] == ' ' || ptr[len] == '\0')) {
                // Remove variable
                str_remove(expr, ptr - expr, len);

                // Replace variable with value
                char value[25];
                int32 value_length = float_to_str(variables[i].value, value, 4);
                str_insert(expr, ptr - expr, value);

                --available_variables;
                break;
            }
        }

        // Move past string, this should work regardless of whether we found a variable or not
        while (str_is_alpha(*ptr) && *ptr != '\0') {
            ++ptr;
        }
    }

    // Evaluate math formula
    return evaluator_evaluate_expression(expr);
}

#endif