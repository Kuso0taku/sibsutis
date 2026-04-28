#include <wchar.h>
#include <stdio.h>
#include "matrix2d.h"

int main() {
  srand(time(NULL)); // make seed for random number
                     
  // init matrices
  Matrix2D *matrix1 = matrix2d_construct_default();
  Matrix2D *matrix2 = matrix2d_construct_default();
  Matrix2D *active_matrix = matrix1; // current matrix
  Matrix2D *other_matrix = matrix2; // second matrix
  
  size_t choice = 0;
  size_t sub_choice = 0;
  size_t matrix_choice = 1;
  
  wint_t code = 0;
  fputws("This is an app for testing work with (double) matrices.\n\n", stdout);
  do {
    fputws("Choose what to do:\n", stdout);
    fputws("(1)  Compare two matrices\n", stdout);
    fputws("(2)  Input the matrix\n", stdout);
    fputws("(3)  Print the matrix\n", stdout);
    fputws("(4)  Change the matrix values\n", stdout);
    fputws("(5)  Fill the matrix with random values\n", stdout);
    fputws("(6)  Get the matrix row\n", stdout);
    fputws("(7)  Get the matrix col\n", stdout);
    fputws("(8)  Transpose the matrix\n", stdout);
    fputws("(9)  Find the determinant of the matrix\n", stdout);
    fputws("(10) Find the inverse matrix\n", stdout);
    fputws("(11) Change active matrix\n", stdout);
    fputws("\n(0)  Exit\n", stdout);
    putchar('\n');

    printf("Enter your choice: ");
    while ((code = scanf("%zu", &choice))!=1 || choice>11) {
      if (code == EOF) {
        printf("EOF ERROR! ABORTING.\n");
        matrix2d_destruct(active_matrix);
        matrix2d_destruct(other_matrix);
        return -1;
      }
      while (getchar() != '\n');
      printf("Invalid input! Try again: ");
    }
    
    putchar('\n');
    
    switch (choice) {
      // Exit
      case 0: 
        fputws("Thanks for using me!\n", stdout);
        matrix2d_destruct(active_matrix);
        matrix2d_destruct(other_matrix);
        return 0;

      // Compare
      case 1: 
        fputws("Avaliable compares are:\n", stdout);
        fputws("(1) Greater than\n", stdout);
        fputws("(2) Less than\n", stdout);
        fputws("(3) Equal to\n", stdout);
        fputws("(4) Not equal to\n", stdout);
        fputws("(5) Greater than or equal to\n", stdout);
        fputws("(6) Less than or equal to\n", stdout);
        putchar('\n');

        printf("Enter your choice: ");
        while ((code = scanf("%zu", &sub_choice))!=1 || 
          sub_choice<1 || sub_choice>6) {
          if (code == EOF) {
            printf("EOF ERROR! ABORTING.\n");
            matrix2d_destruct(active_matrix);
            matrix2d_destruct(other_matrix);
            return -1;
          }
          while (getchar() != '\n');
          printf("Invalid input! Try again: ");
        }
        
        _Bool cmp = 0;

        // type of comparing
        switch (sub_choice) {
          case 1: cmp = matrix2d_grtr(active_matrix, other_matrix); break;
          case 2: cmp = matrix2d_less(active_matrix, other_matrix); break;
          case 3: cmp = matrix2d_eql(active_matrix, other_matrix); break;
          case 4: cmp = matrix2d_not_eql(active_matrix, other_matrix); break;
          case 5: cmp = matrix2d_grtr_or_eql(active_matrix, other_matrix); break;
          case 6: cmp = matrix2d_less_or_eql(active_matrix, other_matrix); break;
        }
        
        if (cmp) fputws("True!\n", stdout);
        else fputws("False!\n", stdout);

        break;

      // input
      case 2:
        code = matrix2d_scanf(active_matrix);
        if (code == EOF) {
          matrix2d_destruct(active_matrix);
          matrix2d_destruct(other_matrix);
          return -1;
        }
        break;

      // output
      case 3:
        matrix2d_printf(active_matrix);
        break;

      // change values 
      case 4:
        if (!active_matrix->data) {
          fputws("Oops! The matrix is empty! First, input one\n", stdout);
          break;
        }

        fputws("What do you want to do?\n", stdout);
        fputws("(1) Increment every element in the matrix\n", stdout);
        fputws("(2) Decrement every element in the matrix\n", stdout);
        fputws("(3) Change a specific element in the matrix\n", stdout);
        putchar('\n');

        printf("Enter your choice: ");
        while ((code = scanf("%zu", &sub_choice))!=1 || 
          sub_choice<1 || sub_choice>3) {
          if (code == EOF) {
            printf("EOF ERROR! ABORTING.\n");
            matrix2d_destruct(active_matrix);
            matrix2d_destruct(other_matrix);
            return -1;
          }
          while (getchar() != '\n');
          printf("Invalid input! Try again: ");
        }
        
        // type of change
        switch (sub_choice) {
          case 1: matrix2d_increment(active_matrix); break;
          case 2: matrix2d_decrement(active_matrix); break;
          case 3:
            size_t row=0, col=0;
            double value=0;

            printf("Enter the row INDEX of the element to change: ");
            while ((code = scanf("%zu", &row))!=1 || row>active_matrix->rows) {
              if (code == EOF) {
                printf("EOF ERROR! ABORTING.\n");
                matrix2d_destruct(active_matrix);
                matrix2d_destruct(other_matrix);
                return -1;
              }
              while (getchar() != '\n');
              printf("Invalid input! Try again: ");
            }

            printf("Enter the column INDEX of the element to change: ");
            while ((code = scanf("%zu", &col))!=1 || col>active_matrix->cols) {
              if (code == EOF) {
                printf("EOF ERROR! ABORTING.\n");
                matrix2d_destruct(active_matrix);
                matrix2d_destruct(other_matrix);
                return -1;
              }
              while (getchar() != '\n');
              printf("Invalid input! Try again: ");
            }

            printf("Enter the value of the element to change: ");
            while ((code = scanf("%lf", &value))!=1) {
              if (code == EOF) {
                printf("EOF ERROR! ABORTING.\n");
                matrix2d_destruct(active_matrix);
                matrix2d_destruct(other_matrix);
                return -1;
              }
              while (getchar() != '\n');
              printf("Invalid input! Try again: ");
            }
            
            matrix2d_setter(active_matrix, row, col, value);
            break;
        }

        break;
      
      // fill with random values
      case 5:
        if (!active_matrix->data) {
          fputws("Oops! The matrix is empty! First, input one\n", stdout);
          break;
        }

        double min=0, max=0;

        printf("Enter the minimal value of random number: ");
        while ((code = scanf("%lf", &min))!=1) {
          if (code == EOF) {
            printf("EOF ERROR! ABORTING.\n");
            matrix2d_destruct(active_matrix);
            matrix2d_destruct(other_matrix);
            return -1;
          }
          while (getchar() != '\n');
          printf("Invalid input! Try again: ");
        }
        
        printf("Enter the maximum value of random number: ");
        while ((code = scanf("%lf", &max))!=1 || max<min) {
          if (code == EOF) {
            printf("EOF ERROR! ABORTING.\n");
            matrix2d_destruct(active_matrix);
            matrix2d_destruct(other_matrix);
            return -1;
          }
          while (getchar() != '\n');
          if (max<min) printf("Max value must be greater than or equal to min! ");
          else printf("Invalid input! ");
          printf("Try again: ");
        }
        
        matrix2d_random(active_matrix, min, max);
        break;

      // get row 
      case 6:
        if (!active_matrix->data) {
          fputws("Oops! The matrix is empty! First, input one\n", stdout);
          break;
        }

        printf("Enter the row INDEX of the matrix: ");
        while ((code = scanf("%zu", &sub_choice))!=1 || 
          sub_choice>active_matrix->rows) {
          if (code == EOF) {
            printf("EOF ERROR! ABORTING.\n");
            matrix2d_destruct(active_matrix);
            matrix2d_destruct(other_matrix);
            return -1;
          }
          while (getchar() != '\n');
          printf("Invalid input! Try again: ");
        }

        matrix2d_printf(matrix2d_get_row(active_matrix, sub_choice));
        break;

      // get col
      case 7:
        if (!active_matrix->data) {
          fputws("Oops! The matrix is empty! First, input one\n", stdout);
          break;
        }

        printf("Enter the column INDEX of the matrix: ");
        while ((code = scanf("%zu", &sub_choice))!=1 || 
          sub_choice>active_matrix->rows) {
          if (code == EOF) {
            printf("EOF ERROR! ABORTING.\n");
            matrix2d_destruct(active_matrix);
            matrix2d_destruct(other_matrix);
            return -1;
          }
          while (getchar() != '\n');
          printf("Invalid input! Try again: ");
        }

        matrix2d_printf(matrix2d_get_col(active_matrix, sub_choice));
        break;
      
      // transpose
      case 8:
        if (!active_matrix->data) {
          fputws("Oops! The matrix is empty! First, input one\n", stdout);
          break;
        }

        active_matrix = matrix2d_transpose(active_matrix);
        break;

      // determinant 
      case 9:
        if (!active_matrix->data) {
          fputws("Oops! The matrix is empty! First, input one\n", stdout);
          break;
        }

        if (active_matrix->rows != active_matrix->cols) {
          fputws("Oops! The matrix must be square!\n", stdout);
          break;
        }
        
        printf("The determinant of the matrix is %.2f\n", 
                matrix2d_determinant(active_matrix));
        break;

      // find the inverse matrix 
      case 10:
        if (!active_matrix->data) {
          fputws("Oops! The matrix is empty! First, input one\n", stdout);
          break;
        }

        if (active_matrix->rows != active_matrix->cols) {
          fputws("Oops! The matrix must be square!\n", stdout);
          break;
        }

        if (!matrix2d_determinant(active_matrix)) {
          fputws("Oops! The determinant must not be 0!\n", stdout);
          break;
        }

        matrix2d_printf(matrix2d_inverse(active_matrix));
        break;
      
      // change active matrix
      case 11:
        fputws("Matrices to choose:\n", stdout);

        fputws("matrix(1)", stdout);
        if (matrix_choice == 1) fputws(" - (active)", stdout);
        putchar('\n');
        fputws("matrix(2)", stdout);
        if (matrix_choice == 2) fputws(" - (active)", stdout);
        putchar('\n');
        
        printf("Enter your choice: ");
        while ((code = scanf("%zu", &matrix_choice))!=1 || 
          matrix_choice<1 || matrix_choice>2) {
          if (code == EOF) {
            printf("EOF ERROR! ABORTING.\n");
            matrix2d_destruct(active_matrix);
            matrix2d_destruct(other_matrix);
            return -1;
          }
          while (getchar() != '\n');
          printf("Invalid input! Try again: ");
        }

        if (matrix_choice == 1) active_matrix = matrix1;
        else active_matrix = matrix2;

        printf("Now the matrix(%zu) is active\n", matrix_choice);

        break;
    }

    putchar('\n');
  } while (choice>0 && choice<12);
  
  matrix2d_destruct(active_matrix);
  matrix2d_destruct(other_matrix);
  return 0;
}
