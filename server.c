/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yokitaga <yokitaga@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 18:36:35 by yokitaga          #+#    #+#             */
/*   Updated: 2022/12/17 20:02:01 by yokitaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minitalk.h"
#include <stdio.h>

t_signal    g_signal;

int ft_change_binary(size_t n)
{
    if (n == 0)
        return (1);
    else if (n == 1)
        return (2);
    else
        return (2 * ft_change_binary(n - 1));
}

void signal_handler(int signal, siginfo_t *info, void *context)
{
    //info->si_pidはシグナルを送信したプロセスのIDの値が入っている。
    //今回の場合はclientのプロセスIDが入っている
    const pid_t pid = info->si_pid;
    //char ch;
    
    (void)context;
    //if (signal == ZEROBIT)
        //g_atomic_data[CHAR] = g_atomic_data[CHAR] << 1;
    //else if (signal == ONEBIT)
        //g_atomic_data[CHAR] = g_atomic_data[CHAR] << 1 | 1;
    if (signal == ONEBIT)
        g_signal.nbr += ft_change_binary(g_signal.cnt);
    if (g_signal.cnt == 7 && g_signal.nbr == EOT)
    {
        //printf("%d\n", g_signal.cnt);
        kill(pid, CMPSIG);
        return ;
    }
    else if (g_signal.cnt < 7)
    {
        //printf("%d\n", g_signal.cnt);
        g_signal.cnt++;
    }
    else if (g_signal.cnt == 7)
    {
        ft_putchar_fd((char)g_signal.nbr, 1);
        //ch = (char)ft_change(g_atomic_data[CHAR]);
        //ft_putchar_fd(ch, 1);
        g_signal.nbr = 0;
        g_signal.cnt = 0;
    }
    //kill() 関数は、以下の引数を取ります。
    //- pid_t pid:シグナルを送信するプロセスのIDを指定する。
    //- int sig：送信するシグナルの種類を指定する。
    //kill() 関数は、成功すると 0 を、失敗すると -1 を返します。
    kill(pid, ACKSIG);
}

int main(void)
{
    g_signal.nbr = 0;
    g_signal.cnt = 0;
    //sigaction構造体の宣言
    struct sigaction    sa;
    //sa_flagsはシグナルハンドラ関数の動作を制御するフラグ
    //SA_SIGINFOはsigaction構造体のsa_flagsメンバーに設定するフラグの一つです。
    //SA_SIGINFOフラグを設定すると、シグナルハンドラ関数には、シグナルに関する情報が渡される。
    sa.sa_flags = SA_SIGINFO;
    //シグナルに対する処理を行う関数へのポインタ。この値がNULLの場合は、デフォルトの処理を行う。
    sa.sa_sigaction = &signal_handler;
    //sigemptyset() 関数は、sigset_t 型の引数に指定されたシグナル集合のすべてのシグナルをクリアし、空の状態にする。
    sigemptyset(&sa.sa_mask);
    //sigaddset() 関数は、sigset_t 型の引数に指定されたシグナル集合に、int 型の引数で指定されたシグナルを追加する。
    //sa_maskはシグナルハンドラ関数が実行される間、他のシグナルをブロックするシグナルセット。
    //sa_maskにSIGUSR1とSIGUSR2を追加
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);
    //sigaction() 関数は、シグナルハンドラを登録するための関数です。
    //シグナルハンドラとは、特定のシグナルを受信した際に実行される処理のことです。
    //sigaction() 関数は、引数で指定されたシグナルに対して、引数で指定されたハンドラを登録します。
    //signal_handlerで処理されるシグナルSIGUSR1/2を登録
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    ft_putstr_fd("PID:", 1);
    ft_putnbr_fd(getpid(), 1);
    ft_putstr_fd("\n",1);
    while(1)
        pause();
    return(0);
}