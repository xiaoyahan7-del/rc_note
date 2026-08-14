#ifndef VOFA_UART_H
#define VOFA_UART_H

/* 初始化串口作业模块。 */
void vofa_uart_init(void);

/* 放在 main() 的 while (1) 中反复调用。 */
void vofa_uart_run(void);

#endif
