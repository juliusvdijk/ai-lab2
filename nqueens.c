/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAXQ 100
#define POPULATION_SIZE 10
#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */

void initializeRandomGenerator() {
  /* this routine initializes the random generator. You are not
   * supposed to understand this code. You can simply use it.
   */
	time_t t;
	srand((unsigned) time(&t));
}

/* Generate an initial position.
 * If flag == 0, then for each row, a queen is placed in the first column.
 * If flag == 1, then for each row, a queen is placed in a random column.
 */
void initiateQueens(int flag) {
  int q;
  for (q = 0; q < nqueens; q++) {
    queens[q] = (flag == 0? 0 : rand()%nqueens);
  }
}

/* returns TRUE if position (row0,column0) is in 
 * conflict with (row1,column1), otherwise FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
  if (row0 == row1) return TRUE; /* on same row, */
  if (column0 == column1) return TRUE; /* column, */
  if (ABS(row0-row1) == ABS(column0-column1)) return TRUE;/* diagonal */
  return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in 
 * conflict with any other queen on the board, otherwise FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
  int queen;
  for (queen=0; queen < nqueens; queen++) {
    if (inConflict(row, col, queen, queens[queen])) {
      if ((row != queen) || (col != queens[queen])) return TRUE;
    }
  }
  return FALSE;
}

/* print configuration on screen */
void printState() {
  int row, column; 
  printf("\n");
  for(row = 0; row < nqueens; row++) {
    for(column = 0; column < nqueens; column++) {
      if (queens[row] != column) {
        printf (".");
      } else {
        if (inConflictWithAnotherQueen(row, column)) {
          printf("Q");
        } else {
          printf("q");
        }
      }
    }
    printf("\n");
  }
}

/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if ((column < 0) || (column >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: column=%d "
      "(should be 0<=column<%d)...Abort.\n", column, nqueens);
    exit(-1);
  }
  queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
 * (queen,column). Note that this routine checks only that 
 * the values of queen and column are valid! It does not test
 * conflicts!
 */
int canMoveTo(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in canMoveTo: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if(column < 0 || column >= nqueens) return FALSE;
  if (queens[queen] == column) return FALSE; /* queen already there */
  return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in columnOfQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, queens[queen], other, queens[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

/* evaluation function. The maximal number of queens in conflict
 * can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * Since we want to do ascending local searches, the evaluation
 * function returns (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
  return (nqueens-1)*nqueens/2 - countConflicts();
}

/*************************************************************/


void saveState(int* successor){
	for(int i = 0; i < nqueens; i++){
		successor[i] = queens[i];
	}
}

void loadState(int* successor){
	for(int i = 0; i < nqueens; i++){
		queens[i] = successor[i];
	}
}

int nextSuccessor() {
	int score = evaluateState();
    int moveQueenIdx = -1;
    int moveQueenNewPos = 0;

    for (int i = 0; i < nqueens; i++) {
        int pos = queens[i];

        for (int nextPos = 0; nextPos < nqueens; nextPos++) {
            if (nextPos == pos) continue;

            queens[i] = nextPos;
            int newScore = evaluateState();
            // We have a 10% chance to throw away the current best move if the score is equal
            if (newScore > score || (newScore == score && (rand() < (RAND_MAX / 10)))) {
                score = newScore;
                moveQueenIdx = i;
                moveQueenNewPos = nextPos;
            }
        }

        // Reset move
        queens[i] = pos;
    }

    if (moveQueenIdx == -1) {
        // No change
        return 0;
    } else {
        queens[moveQueenIdx] = moveQueenNewPos;
        return 1;
    }
}


void randomSuccessor(int* next){
	//choosing a random row and column to change for the next state
	int row = (rand() / (RAND_MAX / nqueens));
	int column;
	
	for(int i = 0; i < nqueens; i++){
		next[i] = queens[i];
		if(i == row){
			do{
				column = (rand() / (RAND_MAX / nqueens));
			}while(column == next[i]);
			next[i] = column;
		}
	}
}

/*************************************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 40
void randomSearch() {
  int queen, iter = 0;
  int optimum = (nqueens-1)*nqueens/2;

  while (evaluateState() != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER) break;  /* give up */
    /* generate a (new) random state: for each queen do ...*/
    for (queen=0; queen < nqueens; queen++) {
      int pos, newpos;
      /* position (=column) of queen */
      pos = columnOfQueen(queen);
      /* change in random new location */
      newpos = pos;
      while (newpos == pos) {
        newpos = rand() % nqueens;
      }
      moveQueen(queen, newpos);
    }
  }
  if (iter < MAXITER) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
}


void printGen(int** Gen){
	for(int i = 0; i < nqueens; i++){
		for(int j = 0; j < nqueens; j++){
			printf("%d ", Gen[i][j]);
		}
		printf("rand = %d \n", rand());
		
	}

}
/*************************************************************/
void hillClimbing() {
	int eval, i;
	eval = nextSuccessor();
	for(i = 2; eval != ((nqueens-1)*nqueens/2) ; i++){
		if (nextSuccessor() == 0) {
			// Random restart
            initiateQueens(1);
		}
		eval = evaluateState();
	}
	printf ("Solved puzzle in %d iterations.\n", i);
	printf ("Final state is");
	printState();


}
/*************************************************************/

double prop(int deltaE, double temperature){
	return (exp(deltaE/temperature));
}

double calculateTemperature(int t, int timeMax) {
	//return (1.0 - (double)t/timeMax) * 5;
	//double component = 1.0 / ((double)t/timeMax);
	
	
	//1.0/t is the best thus far
	return (1.0/t);
}

double fRandom() {
	return rand()/(double)RAND_MAX;
}

void simulatedAnnealing() {
	int t, deltaE, currentValue, timeMax = 100000;
	int nextState[nqueens], currentState[nqueens];

	double temperature;

	for(t = 1; t < timeMax; t++){
		saveState(currentState);
		currentValue = evaluateState();
		temperature = calculateTemperature(t, timeMax);
		if(temperature == 0 || currentValue == (nqueens-1)*nqueens/2){
			break;
		}
		
		//Saving a successor of the current state in nextState
		randomSuccessor(nextState);
		loadState(nextState);
		deltaE = evaluateState() - currentValue;
		if(deltaE > 0){
			loadState(nextState);
		}else if(prop(deltaE, temperature) >= fRandom()){
			loadState(nextState);
		} else {
			loadState(currentState);
		}
		
	}
	printf("Conflicts %d\n", countConflicts());
	printf ("Solved puzzle in %d iterations.\n", t);
	printf ("Final state is");
	printState();

}


/*************************************************************/
typedef struct {
    int* queens;
} gIndividual;

gIndividual newIndividual(int* baseState) {
    gIndividual ind;
    ind.queens = malloc(sizeof(int) * nqueens);
    memcpy(ind.queens, baseState, sizeof(int) * nqueens);
    return ind;
}

int cmpIndividualByFitness(const void* a, const void* b) {
    loadState(((gIndividual*)a)->queens);
    int scoreA = countConflicts();
    loadState(((gIndividual*)b)->queens);
    int scoreB = countConflicts();
    return scoreA - scoreB;
}

typedef struct {
    int a;
    int b;
} CrossOverLocation;

int generateNumberWithBias() {
    return rand() % POPULATION_SIZE;
}

CrossOverLocation generateCrossOverLocation() {
    CrossOverLocation loc;
    loc.a = generateNumberWithBias();
    do {
        loc.b = generateNumberWithBias();
    } while (loc.a == loc.b);

    return loc;
}

void doCrossOver(gIndividual a, gIndividual b, gIndividual out) {
    // Choose random location in the "string"
    int randomLoc = rand() % (nqueens - 1) + 1;
    // Copy A
    memcpy(out.queens, a.queens, sizeof(int) * nqueens);
    // Do crossover
    if (rand() % 2) {
        // Cross heads
        for (int i = 0; i < randomLoc; i++) {
            out.queens[i] = b.queens[i];
        }
    } else {
        // Cross tails
        for (int i = nqueens/2; i >= randomLoc; i--) {
            out.queens[i] = b.queens[i];
        }
    }
}

void geneticAlgorithm() {
    gIndividual populationA[POPULATION_SIZE];
    gIndividual populationB[POPULATION_SIZE];
    gIndividual* currentPopulation = populationA;
    gIndividual* nextPopulation = populationB;

    // Initialize the population with random boards
    for (int i = 0; i < POPULATION_SIZE; i++) {
        populationA[i] = newIndividual(queens);
        populationB[i] = newIndividual(queens);
        initiateQueens(1);
    }

    do {
        // Sort population by their fitness in ascending order (0 is best)
        qsort(currentPopulation, POPULATION_SIZE, sizeof(gIndividual), cmpIndividualByFitness);
        // Check if we have a solution (fitness of idx 0 == 0)
        loadState(currentPopulation[0].queens);
        if (countConflicts() == 0) {
            break;
        }

        // Keep the 2 most fit ones
        // Copy first 2
        for (int i = 0; i < 2; i++) {
            memcpy(nextPopulation[i].queens, currentPopulation[i].queens, sizeof(int) * nqueens);
        }
        // Crossover last POPULATION_SIZE - 2
        for (int i = 2; i < POPULATION_SIZE; i++) {
            CrossOverLocation location = generateCrossOverLocation();
            doCrossOver(currentPopulation[location.a], currentPopulation[location.b],
                        nextPopulation[i]);
        }

        // Apply mutations
        for (int i = 1; i < POPULATION_SIZE; i++) {
            int numberOfMutations = rand() % 3;
            for (int nM = 0; nM < numberOfMutations; nM++) {
                nextPopulation[i].queens[rand() % nqueens] = rand() % nqueens;
            }
        }

        gIndividual* swap = currentPopulation;
        currentPopulation = nextPopulation;
        nextPopulation = swap;
    } while (1);

    printf ("Solved puzzle.\n");
    printf ("Final state is");
    printState();
}


int main(int argc, char *argv[]) {
  int algorithm;

  do {
    printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
    scanf ("%d", &nqueens);
  } while ((nqueens < 1) || (nqueens > MAXQ));

  do {
    printf ("Algorithm: (1) Random search  (2) Hill climbing  ");
    printf ("(3) Simulated Annealing (4) Genetic Algorithm: ");
    scanf ("%d", &algorithm);
  } while ((algorithm < 1) || (algorithm > 4));
  
  initializeRandomGenerator();

  initiateQueens(1);
  
  printf("\nInitial state:");
  printState();

  switch (algorithm) {
  case 1: randomSearch();       break;
  case 2: hillClimbing();       break;
  case 3: simulatedAnnealing(); break;
  case 4: geneticAlgorithm(); break;
  }

  return 0;  
}
