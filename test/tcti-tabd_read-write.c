#include <glib.h>
#include <tcti-tabd.h>
#include <unistd.h>

int
send_recv (TSS2_TCTI_CONTEXT *tcti_context)
{
    /* send / receive */
    TSS2_RC ret;
    char *xmit_str = "test";
    g_debug ("transmitting string: %s", xmit_str);
    ret = tss2_tcti_transmit (tcti_context, strlen (xmit_str), (uint8_t*)xmit_str);
    if (ret != TSS2_RC_SUCCESS)
        g_debug ("tss2_tcti_transmit failed");
    char recv_str[10] = {0};
    size_t recv_size = 10;
    ret = tss2_tcti_receive (tcti_context, &recv_size, recv_str, TSS2_TCTI_TIMEOUT_BLOCK);
    if (ret != TSS2_RC_SUCCESS)
        g_debug ("tss2_tcti_receive failed");
    g_debug ("received string: %s", recv_str);
}

int
send_recv_1024 (TSS2_TCTI_CONTEXT *tcti_context)
{
    /* send / receive */
    TSS2_RC ret;
    uint8_t xmit_buf [1024];
    g_debug ("transmitting 1024 bytes");
    ret = tss2_tcti_transmit (tcti_context, 1024, xmit_buf);
    if (ret != TSS2_RC_SUCCESS)
        g_debug ("tss2_tcti_transmit failed");
    uint8_t recv_buf [1024];
    size_t recv_size;
    ret = tss2_tcti_receive (tcti_context, &recv_size, recv_buf, TSS2_TCTI_TIMEOUT_BLOCK);
    if (ret != TSS2_RC_SUCCESS)
        g_debug ("tss2_tcti_receive failed");
    g_debug ("received %d byte response", recv_size);
}

int
main (void)
{
    TSS2_TCTI_CONTEXT *tcti_context = NULL;
    TSS2_RC ret;
    size_t context_size;

    ret = tss2_tcti_tabd_init (NULL, &context_size);
    if (ret != TSS2_RC_SUCCESS)
        g_error ("failed to get size of tcti context");
    g_debug ("tcti size is: %d", context_size);
    tcti_context = calloc (1, context_size);
    if (tcti_context == NULL)
        g_error ("failed to allocate memory for tcti context");
    g_debug ("context structure allocated successfully");
    ret = tss2_tcti_tabd_init (tcti_context, NULL);
    if (ret != TSS2_RC_SUCCESS)
        g_error ("failed to initialize tcti context");
    g_debug ("initialized");
    /* Wait for initialization thread in the TCTI to setup a connection to the
     * tabd. The right thing to do is for the TCTI to cause callers to block
     * on a mutex till the setup thread is done.
     */
    send_recv (tcti_context);
//    send_recv_1024 (tcti_context);
    tss2_tcti_cancel (tcti_context);
    tss2_tcti_set_locality (tcti_context, 1);
    send_recv (tcti_context);
//    send_recv_1024 (tcti_context);
    tss2_tcti_cancel (tcti_context);
    tss2_tcti_set_locality (tcti_context, 1);
    tss2_tcti_finalize (tcti_context);
}