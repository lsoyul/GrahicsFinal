#pragma once

#include "fps.h"
#include <vector>


#define square_size 200.0f

struct pattern
{
	int length;
	std::vector<int> visibles;
	int start, finish;
	std::vector<int> sequence[200];
	std::vector<int> gaps;
};

pattern pat[11];


pattern loadPat(char *fname);




pattern loadPat(char *fname)
{
	FILE *fp;

	pattern tp;

	int startIndex, finishIndex;
	int pcount = 0;

	fp = fopen(fname, "r");
	if (!fp){
		printf("can't open %s file \n", fname);
		exit(1);
	}

	// read init pattern variables
	int word;
	int fword = fscanf(fp, "%d", &word);
	if (fword == EOF) return tp;
	else{
		tp.length = word;
		fscanf(fp, "%d %d\n", &startIndex, &finishIndex);

		while (1)
		{
			int vis, c;
			int fword = fscanf(fp, "%d", &vis);
			c = fgetc(fp);
			if (c != '\n') tp.visibles.push_back(vis);
			else {
				tp.visibles.push_back(vis); break;
			}
		}

		tp.start = startIndex;
		tp.finish = finishIndex;
		printf("%d %d %d\n", tp.length, tp.start, tp.finish);
	}

	// read patterns
	while (1){
		int seq, c, gap;
		// read first word
		int fword = fscanf(fp, "%d", &seq);
		c = fgetc(fp);
		if (fword == EOF) break;
		else{
			if (c == '\n') {
				tp.sequence[pcount].push_back(seq);
				fscanf(fp, "%d\n", &gap);
				tp.gaps.push_back(gap);
				pcount++;
			}
			else {
				tp.sequence[pcount].push_back(seq);
			}
		}
	}

	for (int i = 0; i < pcount; i++){
		for (uint j = 0; j < tp.sequence[i].size(); j++){
			printf("%d", tp.sequence[i][j]);
		}
		printf("\n%d\n", tp.gaps[i]);
	}


	return tp;

	fclose(fp);
}
