I did Matrix multiplication program with the help of both methods,
1) Collective Communication
	Here the time taken by the program to run :- is 
	Here the matrix i took is, 9*9,

		Here the N = 3(No. of process):-
		Difference in Time(Rank) :1     0.001301
		Difference in Time(Rank) :2     0.001287
		Difference in Time(Rank) :0     0.001288
		Difference in Time(Rank) Overall:0        0.001449

		When the N = 9(No. of Process):-
		Difference in Time(Rank) :1     0.015082
		Difference in Time(Rank) :3     0.015010
		Difference in Time(Rank) :2     0.015242
		Difference in Time(Rank) :4     0.015242
		Difference in Time(Rank) :6     0.016388
		Difference in Time(Rank) :5     0.015534
		Difference in Time(Rank) :8     0.015223
		Difference in Time(Rank) :7     0.016625
		Difference in Time(Rank) :0     0.014586
		Difference in Time(Rank) Overall:0        0.014727

2) Blocking Communication
	Here the time taken by the program to run :-
	Here the matrix I took was, 32*4

		Here the N = 5(No. of Processes):-
		Difference in Time : 0.000621
		Difference in Time : 0.000516
		Difference in Time : 0.000447
		Difference in Time : 0.000485
		
		Time elapsed for complete Program to run : 0.002571

		Now I changed N to 9,
		Difference in Time : 0.003328
		Difference in Time : 0.002017
		Difference in Time : 0.001176
		Difference in Time : 0.001479
		Difference in Time : 0.001764		
		Difference in Time : 0.001229
		Difference in Time : 0.000557
		Difference in Time : 0.000512

		Time elapsed for complete Process : 0.012747

		When 9*9 matrix was used,
		Time notations were,(N = 3)
		Difference in Time : 0.000885
		Difference in Time : 0.000822
		Difference in Time : 0.001212

Observation :- 

Recieve and send Method   :- Time elapsed for complete Process : 0.002837
Scatter and Gather Method :- Difference in Time(Rank) Overall  : 0.001449

It took less time in Scatter and Gather, provided same number of program and solution.

It take more amount of time when the processes are increased in a program. 
This observation was same in both the cases.