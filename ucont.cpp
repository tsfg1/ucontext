#include <stdio.h>
#include <ucontext.h>
#include <stdlib.h>

#define STACK_SIZE 4096

ucontext_t main_context, child_context;

void child_function() {
    printf("Child: Start\n");
    printf("Child: Yield to Main\n");
    swapcontext(&child_context, &main_context); // 切换到主协程
    printf("Child: Resume from Main\n");
    printf("Child: Yield to Main Again\n");
    swapcontext(&child_context, &main_context); // 再次切换到主协程
    printf("Child: End\n");
}

int main() {
    char child_stack[STACK_SIZE];

    // 初始化主协程
    getcontext(&child_context);
    child_context.uc_stack.ss_sp = child_stack;
    child_context.uc_stack.ss_size = sizeof(child_stack);
    child_context.uc_link = &main_context; // 设置返回的上下文为主协程
    makecontext(&child_context, child_function, 0); // 创建子协程的上下文

    printf("Main: Start\n");
    printf("Main: Yield to Child\n");
    swapcontext(&main_context, &child_context); // 切换到子协程
    printf("Main: Resume from Child\n");
    printf("Main: Yield to Child Again\n");
    swapcontext(&main_context, &child_context); // 再次切换到子协程
    printf("Main: End\n");

    // 确保在程序结束前切换回子协程
    swapcontext(&main_context, &child_context);

    return 0;
}

