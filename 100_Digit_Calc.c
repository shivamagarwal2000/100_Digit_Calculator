/* Algorithms are fun!
 * Extended precision integer calculator
 * Implements +, *, and ^ (power of) operations
 * Code written and modified by Shivam Agarwal
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INT_SIZE    100     /* max number of digits per integer value */
#define LINE_LEN    103     /* maximum length of any input line */
#define NUM_VARS    10      /* number of different huge int "variables" */


#define ASN_OP        '='   /* assignment operator */
#define ECH_OP        '?'   /* echo operator */
#define ADD_OP        '+'   /* addition operator */
#define MUL_OP        '*'   /* multiplication operator */
#define POW_OP        '^'   /* power of operator */

#define OPR1_POS    1       /* position of the first operand */
#define OPR2_POS    3       /* position of the second operand */
#define OP_POS      2       /* position of the operator */

#define CH_ZERO        '0'  /* character 0 */

#define EXIT_CMD     "exit" /* command to exit */
#define PROMPT        "> "  /* command prompt */
#define CMT_FLAG      '%'   /* indicator for comment line */

typedef int digit_t;        /* a decimal digit */
typedef digit_t huge_t[INT_SIZE];   /* one huge int "variable" */

/* add your constant and type definitions here */

#define COPY_SIZE   2       /* size of the copy array */

/****************************************************************/

/* function prototypes */
void read_line (char *line, int max_len);
void init (huge_t vars[], int lens[]);
void echo (huge_t vars[], int lens[], int opr1_index);
void assign (huge_t vars[], int lens[], int opr1_index, char *opr2_str);
void add (huge_t vars[], int lens[], int opr1_index, char *opr2_str);
int  multiply (huge_t vars[], int lens[], int opr1_index, char *opr2_str);
void power (huge_t vars[], int lens[], int opr1_index, char *opr2_str);

/* add your function prototypes here */
void put_zero (huge_t vars);
int input_type_checker (char input_first_char);
int set_index (char char_index);
void straight_copier (huge_t arr1, huge_t arr2, int len_arr2);
void reverse_copier (huge_t arr1, char *arr2, int len_arr2);
int sum_calculation (huge_t augent, huge_t addend, int max_len);
int set_maxlength (int first_len, int second_len);
void overflow_handler (huge_t vars, int lens[], int first);
int multiply_single_digits (huge_t vars[], int digit, int first_index,
                huge_t copy[], int lens[], int pos);
void arraycopier (huge_t first_array, huge_t second_array, int second_len);



/****************************************************************/

/* main function controls all the action, do NOT modify this function */
int
main (int argc, char *argv[])
{
  char line[LINE_LEN + 1];  /* to hold the input line */
  huge_t vars[NUM_VARS];    /* to hold 10 huge integers */
  int lens[NUM_VARS];       /* to hold the length of the 10 vars */

  int opr1_index;       /* index of the first operand in command */
  char op;          /* operator in command */

  init (vars, lens);

  while (1)
    {
      printf (PROMPT);      /* print prompt */
      read_line (line, LINE_LEN);   /* read one line of command */

      if (line[0] == CMT_FLAG)
    {           /* print comment in the test data */
      printf ("%s\n", line);    /* used to simplify marking */
      continue;
    }

      if (strcmp (line, EXIT_CMD) == 0)
    {           /* see if command is "exit" */
      return 0;
    }

      opr1_index = line[OPR1_POS] - CH_ZERO;    /* first var number at line[1] */
      op = line[OP_POS];    /* operator at line[2] */

      if (op == ECH_OP)
    {           /* print out the variable */
      echo (vars, lens, opr1_index);
      continue;
    }

      /* do the calculation, second operand starts at line[3] */
      if (op == ASN_OP)
    {
      assign (vars, lens, opr1_index, line + OPR2_POS);
    }
      else if (op == ADD_OP)
    {
      add (vars, lens, opr1_index, line + OPR2_POS);
    }
      else if (op == MUL_OP)
    {
      multiply (vars, lens, opr1_index, line + OPR2_POS);
    }
      else if (op == POW_OP)
    {
      power (vars, lens, opr1_index, line + OPR2_POS);
    }
    }

  /* all done; take some rest */
  return 0;
}

/* read a line of input into the array passed as argument */
void
read_line (char *line, int max_len)
{
  int i = 0, c;
  while (((c = getchar ()) != EOF) && (c != '\n') && (c != '\r'))
    {
      if (i < max_len)
    {
      line[i++] = c;
    }
      else
    {
      printf ("Invalid input line, toooooooo long.\n");
      exit (0);
    }
    }
  line[i] = '\0';
}

/* print out a huge integer */
void
echo (huge_t vars[], int lens[], int opr1_index)
{
  int i;

  /* print the digits in a reverse order */
  for (i = lens[opr1_index] - 1; i >= 0; i--)
    {
      printf ("%d", vars[opr1_index][i]);
    }

  printf ("\n");
}


/****************************************************************/

/* add code below to complete the function bodies,
 * add more functions where appropriate.
 */

/* set the vars array to zeros */

void
init (huge_t vars[], int lens[])
{
  int i;
  for (i = 0; i < NUM_VARS; i++)
    {
      put_zero (vars[i]); // To put each element zero

      lens[i] = 1; // put length of each element as 1
    }

}

/* process the '=' operator */
void
assign (huge_t vars[], int lens[], int opr1_index, char *opr2_str)
{
  int first = opr1_index;

  if (input_type_checker (opr2_str[0])) // checks form of input
    {
      int second = 0;
      // to calculate the index of input variable
      second = set_index (opr2_str[1]); 
      
      lens[first] = lens[second];
      // to copy the values
      straight_copier (vars[first], vars[second], lens[first]);

    }

  else
    {

      lens[first] = strlen (opr2_str);
      // to copy the string in reverse order
      reverse_copier (vars[first], opr2_str, lens[first]);

    }


}

/* process the '+' operator */
void
add (huge_t vars[], int lens[], int opr1_index, char *opr2_str)
{
  int first = opr1_index;
  int second, max_len = 0;

  if (input_type_checker (opr2_str[0]))
    {
      second = set_index (opr2_str[1]);
      // calculate maximum length out of two variables
      max_len = set_maxlength (lens[first], lens[second]);
      // to do actual sum and upadate length
      lens[first] = sum_calculation (vars[first], vars[second], max_len);
    }
  else
    {
      huge_t copy; // make a temporary variable

      put_zero (copy);

      reverse_copier (copy, opr2_str, strlen (opr2_str));

      max_len = set_maxlength (lens[first], strlen (opr2_str));

      lens[first] = sum_calculation (vars[first], copy, max_len);
    }

  overflow_handler (vars[first], lens, first); // to handle overflow
}

/* process the '*' operator */


int
multiply (huge_t vars[], int lens[], int opr1_index, char *opr2_str)
{

  int i, j = 0;
  int copy[COPY_SIZE][INT_SIZE] = { {0} }; //initialise a temp array 0
  int integer_dig = 0, copy_first_len, copy_second_len = 0;
  int max_len, first = opr1_index;


  if (!(input_type_checker (opr2_str[0])))
    {
        // To extract each digit in reverse order
      for (i = strlen (opr2_str) - 1; i >= 0; i--)
    {
      integer_dig = opr2_str[i] - CH_ZERO; // to store digit
      // multiply single digits with number
      copy_first_len =
        multiply_single_digits (vars, integer_dig, first, copy, lens, j);

      max_len = set_maxlength (copy_first_len, copy_second_len);
      //calculate sum of answers and store in copy array
      copy_second_len =
        sum_calculation (copy[COPY_SIZE - 2], copy[COPY_SIZE - 1],
                 max_len);
        // reinitialize the copy array to zero
      put_zero (copy[COPY_SIZE - 1]);
      j++;
    }
      // to retransfer the answer from copy to original array
      arraycopier (vars[first], copy[COPY_SIZE - 2], copy_second_len);
      lens[first] = copy_second_len;

    }

  else
    {
      int second = 0;

      second = set_index (opr2_str[1]);
      for (i = 0; i < lens[second]; i++)
    {
      integer_dig = vars[second][i];
      copy_first_len =
        multiply_single_digits (vars, integer_dig, first, copy, lens, j);
      max_len = set_maxlength (copy_first_len, copy_second_len);
      copy_second_len =
        sum_calculation (copy[COPY_SIZE - 2], copy[COPY_SIZE - 1],
                 max_len);
      put_zero (copy[COPY_SIZE - 1]);
      j++;
    }
      arraycopier (vars[first], copy[COPY_SIZE - 2], copy_second_len);
      lens[first] = copy_second_len;
    }
    
    overflow_handler (vars[first], lens, first);
    return copy_second_len;
}

/* process the '^' operator */
void
power (huge_t vars[], int lens[], int opr1_index, char *opr2_str)
{
  int first = opr1_index, i, number = 0;
  char copy1[100];
  if (!(input_type_checker (opr2_str[0])))
  {
    for(i = 0; i < strlen(opr2_str); i++)
    {
      number = (opr2_str[i] - CH_ZERO) + (number * 10) ; 

    }
    int j = lens[first] - 1;
    for(i = 0; i < lens[first]; i++)
    {
      copy1[i] = (char)(vars[first][j] + CH_ZERO);
      j--;
    }
    copy1[i] = '\0';
    for(i = 1; i < number; i++)
    {
      lens[first] = multiply(vars, lens, first, copy1);
    }
  }

  else
  {
    int second = opr2_str[1] - CH_ZERO;
    for(i = lens[second] - 1; i >= 0; i--)
    {
      number = vars[second][i] + (number * 10) ;
    }
    int j = lens[first] - 1;
    for(i = 0; i < lens[first]; i++)
    {
      copy1[i] = (char)(vars[first][j] + CH_ZERO);
      j--;
    }
    copy1[i] = '\0';
    for(i = 1; i < number; i++)
    {
      lens[first] = multiply(vars, lens, first, copy1);
    }
  }

}

// put every element zero
void
put_zero (huge_t vars)
{
  int i;
  for (i = 0; i < INT_SIZE; i++)
    {
      vars[i] = 0;

    }

}

// checks the form in which input is given
int
input_type_checker (char input_first_char)
{

  if (input_first_char == 'n')
    {

      return 1;
    }

  else
    {
      return 0;

    }


}

// converts character into integer
int
set_index (char char_index)
{
  int integer_index = char_index - CH_ZERO;
  return integer_index;
}

// copies one integer array into another
void
straight_copier (huge_t arr1, huge_t arr2, int len_arr2)
{
  int i;
  for (i = 0; i < len_arr2; i++)
    {
      arr1[i] = arr2[i];
    }
}

// reverse the string and puts it in integer array
void
reverse_copier (huge_t arr1, char *arr2, int len_arr2)
{
  int i, j = len_arr2 - 1;
  for (i = 0; i < len_arr2; i++)
    {
      arr1[i] = set_index (arr2[j]);
      j--;
    }
}

// calculates the sum of 2 numbers and returns the length of answer
int
sum_calculation (huge_t augent, huge_t addend, int max_len)
{

  int i = 0, carry = 0;


  for (i = 0; i < max_len; i++)
    {
      augent[i] += addend[i] + carry;
      if (augent[i] > 9)
    {
      carry = augent[i] / 10;
      augent[i] = augent[i] % 10;

    }
      else
    {
      carry = 0;

    }

    }
  if (carry == 0)
    {
      return max_len;
    }
  else
    {
      augent[i] += carry;
      return (max_len + 1);
    }

}

// calculates greater length and returns it
int
set_maxlength (int first_len, int second_len)
{
  int max_len = first_len;
  if (second_len > first_len)
    {
      max_len = second_len;
    }
  return max_len;

}

//if length exceeds 99 removes trailing zeroes and sets length 
void
overflow_handler (huge_t vars, int lens[], int first)
{
  if (lens[first] > 99)
    {
      int i;
      for (i = (INT_SIZE - 1); i > 0; i--)
    {
      if (vars[i] != 0)
        {
          break;
        }
    }
      lens[first] = i + 1;
    }

}

/* This function multiplies single digit with the vars number and
stores the answer in temp array and returns the length of answer*/
int
multiply_single_digits (huge_t vars[], int digit, int first_index,
            huge_t copy[], int lens[], int pos)
{
  int i, k, carry = 0, new_len;
  for (i = pos, k = 0; i < (lens[first_index] + pos) && k < lens[first_index];
       i++, k++)
    {
      copy[COPY_SIZE - 1][i] = (vars[first_index][k] * digit) + carry;
      if (copy[COPY_SIZE - 1][i] > 9)
    {
      carry = copy[COPY_SIZE - 1][i] / 10;
      copy[COPY_SIZE - 1][i] = copy[COPY_SIZE - 1][i] % 10;

    }
      else
    {
      carry = 0;

    }

    }

  if (carry != 0)
    {
      copy[COPY_SIZE - 1][i] += carry;
      new_len = i + 1;

    }
  else
    {
      new_len = i;
    }

  return new_len;
}
// To transfer back the answer in the vars array from temp array
void
arraycopier (huge_t first_array, huge_t second_array, int second_len)
{
  int i;

  for (i = 0; i < second_len; i++)
    {
      first_array[i] = second_array[i];
    }

}
