#ifndef VOFA_UART_H
#define VOFA_UART_H

/* 初始化串口功能模块。 */
void vofa_uart_init(void);

/*
 * 运行串口功能模块。
 * 这个函数需要放在 main() 的 while (1) 中反复调用。
 */
void vofa_uart_run(void);

#endif
