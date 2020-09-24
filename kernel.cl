kernel void testkernel(global int *A, global int *B, global int *C)
{
    const int idx = get_global_id(0);
    C[idx] = A[idx] + B[idx];
    printf("%i + %i = %i \n", A[idx], B[idx], A[idx] + B[idx]);
}                                 