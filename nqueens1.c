/* Randomly changes one queen in every row in each nextGen*/
void createNextGen(int** nextGen){
	for(int i = 0; i < nqueens; i++){
		do{
				nextGen[i][i] = (rand() % nqueens);
		}while(queens[i] == nextGen[i][i]);
	}
	/*
	for(int i = 0; i < nqueens; i++){
		for(int j = 0; j < nqueens; j++){
			printf("%d ", nextGen[i][j]);
		}
		printf("rand = %d \n", rand());
	} 
	* */
}


/* countConflicts with an array as input */
int countConflicts2(int* newQueens) {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, newQueens[queen], other, newQueens[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}




/* evaluates which nextGen is the best successor */
int evaluateBestNeighbour(int** nextGen){
	int score, remind, currentBest = 0;
	for(int i = 0; i < nqueens; i++){
		
		score = (nqueens-1)*nqueens/2 - countConflicts2(nextGen[i]);
		/* If the new score is better than the previous this score and 
		 * Gen will be saved */
		if(score > currentBest){
			remind = i;
			currentBest = score;	
		}
		/* If the score is the same is the previous we choose one 
		 * of both */
		if(score == currentBest && (rand() % 2)){
			remind = i;
			currentBest = score;
		}
	}
	

	if(evaluateState() <= currentBest){
		return(remind);
	}else{
	return(-1);}
}
/* initiallize nextGen */
void initializeNextGen(int** nextGen){
	for(int i = 0; i < nqueens; i++){
		for(int j = 0; j < nqueens; j++){
			nextGen[i][j] = queens[j];
		}
	}

}
