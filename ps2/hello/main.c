/*
 * Teste mínimo de compilação para PlayStation 2.
 * Baseado no exemplo helloworld do PS2SDK:
 * https://github.com/ps2dev/ps2sdk/tree/master/ee/debug/samples/helloworld
 *
 * Este programa não contém ROMs, recursos do Smash Remix ou código do jogo.
 */

#include <sifrpc.h>
#include <debug.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    sceSifInitRpc(0);
    init_scr();

    scr_printf("Smash Remix PS2 - teste de toolchain\n");
    scr_printf("PS2SDK ELF smoke test: OK\n");

    sleep(5);
    scr_clear();
    scr_setXY(10, 10);
    scr_printf("Primeiro ELF nativo compilado na nuvem.\n");

    sleep(10);
    return 0;
}
