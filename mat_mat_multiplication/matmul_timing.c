#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../util/parse_matrix_file/read_matrix_file.h"
#include "../util/format_datatypes/COO.h"
#include "../util/format_datatypes/CSR.h"
#include "../util/format_datatypes/CSC.h"

void matmat(){
    //MPI
    
    int proc_offset_A, proc_offset_B;
    if(rank == 0){
        proc_offset_A = 0;
        proc_offset_B = 0;
    }
    else{
        proc_offset_A = n/2;
        proc_offset_B = n/2;
    }
    
    int non_zero_val_counter = 0;
    for(int i = 0; i<2; i++){
        for(int row = proc_offset_A, row < proc_offset_A+ n/2; row++){
            for(int col = proc_offset_B, col < proc_offset_B + n/2 ; col++){
                double tmp;
                int A_val_index, B_val_index;
                for(int j=0; j < n ; j++){
                    for(int k=0; k < coo_A.nnz; k++){
                        if (coo_A.rows[k] = row && coo_A.cols[k] == j){
                            A_val_index = k;
                            if(B_val_index != 0){break;}
                        }
                        if (coo_B.rows[k]==j && coo_B.cols[k] = col){
                            B_val_index = k;
                            if(A_val_index != 0) { break;}
                        }
                    }

                    if(A_val_index && B_val_index){
                        tmp += coo_A.vals[A_val_index] * coo_B.vals[B_val_index];
                    }
                }
                coo_C.cols[non_zero_val_counter] = col;
                coo_C.rows[non_zero_val_counter] = row;
                coo_C.vals[non_zero_val_counter] = tmp;
                non_zero_val_counter++;
            }
        }
        //MPI_Barrier before next iteration
        if(rank ==0){
            proc_offset_B = n/2;
        }
        else{
            proc_offset_B = 0;
        }
    }
}
