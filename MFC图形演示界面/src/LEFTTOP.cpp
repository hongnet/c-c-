// LEFTTOP.cpp : 实现文件
//

#include "stdafx.h"
#include "testsystem1.h"
#include "LEFTTOP.h"
//#include "stdio.h"
//#include "stdlib.h"
//#include "math.h"
#include"random.h"
#include"testsystem1Doc.h"
#include"datalist.h"
#include"testdlg.h"
//extern bool issymod;
/********************************************************************/
extern int  sc, alogrithm,popn,gennumber, bdu;
extern MYSQL m_mysql;
extern CString char2CSt(char * szBuf);
extern char * CSt2char(CString str);
extern int zs, pn, gn, zqs, das;
// CLEFTTOP
CLEFTTOP *lft;
IMPLEMENT_DYNCREATE(CLEFTTOP, CFormView)

CLEFTTOP::CLEFTTOP()
	: CFormView(CLEFTTOP::IDD)
	, sysnum(0)
	, pop(0)
   , gen1(0)
{
	lft = this;
}

CLEFTTOP::~CLEFTTOP()
{
}

void CLEFTTOP::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_COMBO1, sysnum);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT4, m_edit4);
	DDX_Control(pDX, IDC_EDIT5, m_edit5);
	DDX_Control(pDX, IDC_EDIT6, m_edit6);
	DDX_Control(pDX, IDC_EDIT7, m_edit7);
	DDX_Control(pDX, IDC_EDIT9, timelimit);
	DDX_Control(pDX, IDC_EDIT10, time0);
	DDX_Control(pDX, IDC_COMBO2, m_alogcombox);
	//  DDX_Control(pDX, IDC_COMBO3, pop);
	//  DDX_Control(pDX, IDC_COMBO4, gen);

	//DDX_Control(pDX, IDC_COMBO3, pop);
	//DDX_Control(pDX, IDC_COMBO4, gen);

	DDX_CBIndex(pDX, IDC_COMBO1, sysnum);
	DDX_CBIndex(pDX, IDC_COMBO3, pop);
	DDX_CBIndex(pDX, IDC_COMBO4, gen1);

	DDX_Control(pDX, IDC_BUTTON3, m_duibi);
	DDX_Control(pDX, IDC_BUTTON1, m_sysmodel);
}

BEGIN_MESSAGE_MAP(CLEFTTOP, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &CLEFTTOP::OnBnClickedButalog)
//	ON_CBN_SELCHANGE(IDC_COMBO2, &CLEFTTOP::OnCbnSelchangeCombo2)
//	ON_CBN_SELCHANGE(IDC_COMBO1, &CLEFTTOP::OnCbnSelchangeCombo1)
//ON_CBN_SELCHANGE(IDC_COMBO1, &CLEFTTOP::OnCbnSelchangeCombo1)
ON_CBN_SELCHANGE(IDC_COMBO1, &CLEFTTOP::OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDC_BUTTON3, &CLEFTTOP::OnBnClickedButton3)
//ON_BN_DOUBLECLICKED(IDC_BUTTON3, &CLEFTTOP::OnBnDoubleclickedButton3)
ON_CBN_SELCHANGE(IDC_COMBO2, &CLEFTTOP::OnCbnSelchangeCombo2)
ON_BN_CLICKED(IDC_BUTTON1, &CLEFTTOP::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON4, &CLEFTTOP::OnBnClickedButton4)
END_MESSAGE_MAP()


// CLEFTTOP 诊断

#ifdef _DEBUG
void CLEFTTOP::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLEFTTOP::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLEFTTOP 消息处理程序
# define INF 1.0e14
# define EPS 1.0e-14
# define PI 3.14159265358979
#define CR 0.9
#define F 0.7
#define maxfun    2 /*Max no. of functions */
#define MAX_EXPERIMENT  30  /*Experimental times*/
#define  MAX_GENERATION 50 /*Max no. of generation*/
#define MAX_GENERATION1  100 /*Max no. of generation*/
#define MAX_GENERATION2  250 /*Max no. of generation*/

#define noperiods 3
#define maxfun1  3
/********************************************************************/

double time_stage; /*the value of testing time in each stage*/

/********************************************************************/
double tperiod[noperiods] = { 30000, 5000, 15000 };//complex时间限制50000
double tperiod1[noperiods] = { 40000, 10000, 20000 };//large时间限制70000
double tperiod2[noperiods] = { 40000, 20000, 30000 };//llarge1时间限制90000
//////////////////////////////////////////////////////////////
/********************************************************************/
int gener;       /*No of generations*/
int nvar;          /*No of variables*/
int ncons;         /*No of Constraints*/
/********************************************************************/

/********************************************************************/
double seed;      /*Random Seed*/
int nfunc; 
double pcross;        /*Cross-over Probability*/
double pmut_r;          /*Mutation Probability*/

double di;       /*Distribution Index for the Cross-over*/
double dim;          /*Distribution Index for the Mutation*/
/*No of functions*/
static int popsize;/*Population Size*/
int Lastrank;
#define MAX_SYSTEM   5//complex system  
#define MAX_SYSTEM1   6//larger system 
#define MAX_SYSTEM2   7//large-scale system 
#define maxvar    10  /*complex*/
#define maxvar1    14  /*larger*/
#define maxvar2    20  /*large-scale*/
#define maxpop 50 /*Max population */
#define maxpop1 100 /*Max population */
#define maxpop2 250 /*Max population */
#define MISSION_TIME 50.0//complex system
#define MISSION_TIME1 100.0//larger system
#define MISSION_TIME2 200.0//large-scale system
#define T_MAX_RESOURCE  50000.0  //complex system
#define T_MAX_RESOURCE1  70000.0  //larger system
#define T_MAX_RESOURCE2 90000.0  //large-scale system
	double err_detect[2][10] = { 0.776220, 0.814637, 0.753587, 0.645021, 0.733427, 0.617261, 0.706967, 0.949077, 0.984545, 0.406537,
		0.889575, 0.929145, 0.790805, 0.817512, 0.993921, 0.770196, 0.749751, 0.861339, 0.899509, 0.945204 };//simple

double err_detect1[2][14] = { 0.717922,0.693995,0.649665,0.488911,0.635522,0.567300,0.705941,0.484446,0.862231,0.556418,0.517790,0.922221,0.581862,0.911374,
                                0.860042,0.979736,0.743828,0.717442,0.806757,0.941858,0.741377,0.915185,0.973388,0.741142,0.995614,0.975707,0.983442,0.723746 };//complex

double err_detect2[2][20] = { 0.668772,0.964836,0.669841,0.697375,0.698799,0.962954,0.859106,0.456550,0.584558,0.604989,0.889336,0.711498,0.472369,0.581896,0.575945,0.517621,0.746188,0.702360,0.666857,0.720739,
                                 0.804824,0.678111,0.913012,0.677043,0.712943,0.918526,0.969919,0.821039,0.722953,0.737724,0.772607,0.874844,0.780714,0.759158,0.900856,0.915738,0.704703,0.876542,0.792779,0.771447 };//larger
double para_module[10][2] = {
	32.841578, 0.006082, //complex system
	33.688620, 0.006062,
	248.547319, 0.000663,
	298.294015, 0.000822,
	232.900479, 0.000895,
	251.756340, 0.000413,
	204.463332, 0.000609,
	280.756554, 0.000344,
	285.828730, 0.000800,
	269.696646, 0.000434 };
double para_module1[14][2] = {

	//larger systems
	32.620930, 0.006082,
	333.497116, 0.000616,
	305.160680, 0.000703,
	345.971557, 0.000514,
	263.141270, 0.000849,
	301.283303, 0.000426,
	287.137974, 0.000327,
	227.329325, 0.000396,
	288.918729, 0.000682,
	206.184576, 0.000443,
	319.017609, 0.000784,
	312.833033, 0.000334,
	309.266945, 0.000712,
	31.791284, 0.005944 };
double para_module2[20][2] = {

	//large-scale system
	32.420270, 0.006197,
	314.998321,0.000898,
	305.252235,0.000753,
	252.269051,0.000896,
	303.823969,0.000546,
	344.964446,0.000578,
	330.301828,0.000617,
	267.082736,0.000829,
	214.246040,0.000538,
	202.224799,0.000479,
	286.025575,0.000703,
	290.813929,0.000608,
	227.228614,0.000807,
	293.340861,0.000871,
	282.564776,0.000829,
	298.307749,0.000857,
	296.865749,0.000309,
	231.197546,0.000562,
	325.325785,0.000891,
	34.714347, 0.005853
	//large-scale1 system
	/*	34.250160, 0.006066,
	246.336253, 0.000378,
	328.237251, 0.000463,
	242.435987, 0.000810,
	303.164769, 0.000309,
	295.661794, 0.000349,
	339.809870, 0.000882,
	226.377148, 0.000804,
	316.330454, 0.000526,
	241.909543, 0.000414,
	333.396405, 0.000372,
	210.744041, 0.000832,
	219.373150, 0.000760,
	346.749779, 0.000790,
	326.067385, 0.000755,
	308.195746, 0.000387,
	207.109287, 0.000889,
	241.163366, 0.000640,
	246.359142, 0.000490,
	322.107608, 0.000650,
	278.289132, 0.000517,
	229.187902, 0.000423,
	267.696158, 0.000874,
	347.157201, 0.000882,
	240.888699, 0.000713,
	285.361797, 0.000729,
	335.497604, 0.000314,
	263.667714, 0.000888,
	261.136204, 0.000424,
	34.864345, 0.005987*/
};
double para_module3[maxvar][5] = {
	32.841578, 0.006082, 3.420495, 6.044874, 4.059062,//complex system
	33.688620, 0.006062, 3.404628, 6.078317, 4.073003,
	248.547319, 0.000663, 3.462033, 6.005200, 5.017264,
	298.294015, 0.000822, 3.467000, 6.019367, 4.975698,
	232.900479, 0.000895, 3.434178, 6.007044, 4.907801,
	251.756340, 0.000413, 3.509075, 6.066268, 4.915339,
	204.463332, 0.000609, 3.518821, 6.193493, 4.961184,
	280.756554, 0.000344, 3.451386, 6.139323, 4.959322,
	285.828730, 0.000800, 3.452095, 6.148070, 5.054851,
	269.696646, 0.000434, 3.450456, 6.137596, 4.997494
};

	//larger systems
double para_module4[maxvar1][5] = {
	32.620930, 0.006082, 3.522588, 6.147295, 4.005109,
	333.497116, 0.000616, 3.422999, 6.035408, 4.970656,
	305.160680, 0.000703, 3.442482, 6.134654, 5.088934,
	345.971557, 0.000514, 3.445503, 6.029518, 4.926746,
	263.141270, 0.000849, 3.487477, 6.193048, 5.029283,
	301.283303, 0.000426, 3.509990, 6.063833, 5.095630,
	287.137974, 0.000327, 3.445375, 6.136485, 4.956111,
	227.329325, 0.000396, 3.454801, 6.054451, 5.033164,
	288.918729, 0.000682, 3.412886, 6.065279, 4.969466,
	206.184576, 0.000443, 3.500281, 6.050063, 5.015519,
	319.017609, 0.000784, 3.478967, 6.074172, 4.996005,
	312.833033, 0.000334, 3.478701, 6.050587, 4.968355,
	309.266945, 0.000712, 3.508663, 6.114560, 5.009268,
	31.791284, 0.005944, 3.496449, 6.127970, 4.047981 };
	//large-scale system
double para_module5[maxvar2][5]={
	32.420270, 0.006197, 3.519663, 6.148448, 4.027271,
	314.998321, 0.000898, 3.514760, 6.159618, 4.908747,
	305.252235, 0.000753, 3.456069, 6.018165, 5.097919,
	252.269051, 0.000896, 3.510778, 6.088308, 4.916584,
	303.823969, 0.000546, 3.501979, 6.182574, 5.029575,
	344.964446, 0.000578, 3.407123, 6.055531, 5.003385,
	330.301828, 0.000617, 3.420403, 6.172765, 5.019047,
	267.082736, 0.000829, 3.508699, 6.182855, 4.943434,
	214.246040, 0.000538, 3.445672, 6.158611, 4.947823,
	202.224799, 0.000479, 3.438600, 6.105075, 4.913196,
	286.025575, 0.000703, 3.547688, 6.061098, 5.063628,
	290.813929, 0.000608, 3.511419, 6.081002, 4.947298,
	227.228614, 0.000807, 3.460257, 6.089798, 5.015751,
	293.340861, 0.000871, 3.522346, 6.097110, 5.047075,
	282.564776, 0.000829, 3.496980, 6.061446, 4.953163,
	298.307749, 0.000857, 3.485426, 6.080966, 5.041063,
	296.865749, 0.000309, 3.502986, 6.156194, 4.965969,
	231.197546, 0.000562, 3.452956, 6.146306, 5.081811,
	325.325785, 0.000891, 3.449463, 6.134001, 5.052532,
	34.714347, 0.005853, 3.483480, 6.049934, 4.094241

};

int int_exp;
int i, j, l, f;
double sumre = 0.0;
double costre = 0.0;
int nsystem;
int no_subsys[5] = { 1, 1, 4, 2, 2 };//complex system
int no_subsys1[6] = { 1, 2, 4, 3, 3, 1 };//larger system
int no_subsys2[7] = { 1, 3, 4, 4, 4,3, 1 };//large-scale system


typedef struct       /*individual properties*/
{
	int rank,              /*Rank of the individual*/
		flag;              /*Flag for ranking*/
	double xreal[20]; /*list of real variables*/
	double fitness[maxfun1];/*Fitness values */
	double cub_len;            /*crowding distance of the individual*/

}individual;        /*Structure defining individual*/

typedef struct
{
	int maxrank;            /*Maximum rank present in the population*/
	double rankrat[250];  /*Rank Ratio*/
	int rankno[250];     /*Individual at different ranks*/
	individual ind[250]; /*Different Individuals*/
	individual *ind_ptr;
}population;             /*Population Structure*/

typedef struct
{
	int maxrank,   /*Max rank of the global population*/
		rankar[2 * 250][2 * 250], /*record of array of individual numbers at a particular rank */
		rankno[2 * 250];           /*record of no. of individuals at a particular rank*/

	int rank[2 * 250],            /*rank of different individuals*/
		flag[2 * 250];            /*Setting the flag */

	double fitness[2 * 250][maxfun1], /*Fitness function values for the different individuals*/
		cub_len[2 * 250],              /*Dummy fitness*/
		xreal[2 * 250][20];       /*value of the decoded variables for different individuals */

}globpop;/*Population structure for the pool having both the old as well as new population*/       /*Structure defining individual*/


	
double fpara1[2 * maxpop2][2];
//double fpara11[2 * maxpop1][2];
//double fpara12[2 * maxpop2][2];

/********************************************************************/

globpop globalpop, *global_pop_ptr;
population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/
//void realinit(population *pop_ptr);/*initializes the population*/
void sort(int m1)
{
	double temp, temp1;
	int i1, k1;
	for (k1 = 0; k1 < m1 - 1; k1++)
	{
		for (i1 = k1 + 1; i1 < m1; i1++)
		{
			if (fpara1[k1][1] >fpara1[i1][1])
			{
				temp = fpara1[k1][1];
				temp1 = fpara1[k1][0];
				fpara1[k1][1] = fpara1[i1][1];
				fpara1[k1][0] = fpara1[i1][0];
				fpara1[i1][1] = temp;
				fpara1[i1][0] = temp1;
			}
		}
	}
};


void realinit(population *pop_ptr)
{
	int i, j;
	double d;

	double sum;
	double rnd;

	for (i = 0; i < popsize; i++)
	{
		sum = 0;

		for (j = 0; j < nvar; j++)
		{
			d = rndreal(-1, 1);

			/*limits are specified it generates the value in range of minimum and maximum value of the variable*/
			pop_ptr->ind[i].xreal[j] = /*randomperc()**/time_stage / nvar - d*(time_stage / nvar) / nvar;

			sum = sum + pop_ptr->ind[i].xreal[j];

		}

		if (sum>time_stage)
		{

			for (j = 0; j < nvar; j++)
			{
				rnd = randomperc();
				pop_ptr->ind[i].xreal[j] = pop_ptr->ind[i].xreal[j] * rnd * (time_stage / sum);
			}

		}

	}

}
void realinitir(population *pop_ptr)
{
	int i, j;
	double d;

	double sum;
	double rnd;

	for (i = 0; i < popsize; i++)
	{
		sum = 0;

		for (j = 0; j < nvar; j++)
		{
			d = randomperc();

			/*limits are specified it generates the value in range of minimum and maximum value of the variable*/
			pop_ptr->ind[i].xreal[j] = d*time_stage;

			sum = sum + pop_ptr->ind[i].xreal[j];

		}

		if (sum>time_stage)
		{

			for (j = 0; j < nvar; j++)
			{
				rnd = randomperc();
				pop_ptr->ind[i].xreal[j] = pop_ptr->ind[i].xreal[j] * rnd * (time_stage / sum);
			}

		}

	}

}
void sleep(clock_t wait)
{
	clock_t goal;
	goal = wait + clock();
	while (goal > clock());
}
int indcmp(double *ptr1, double *ptr2)
{
	double fit1[maxfun1], fit2[maxfun1];
	int i, value, m, n;
	for (i = 0; i < nfunc; i++)
	{
		fit1[i] = *ptr1++;
		fit2[i] = *ptr2++;
	}
	m = 0;
	n = 0;
	while (m < nfunc && fit1[m] <= fit2[m])
	{
		if ((fit2[m] - fit1[m]) < EPS) n++;
		m++;
	}
	if (m == nfunc)
	{
		if (n == nfunc) value = 3;
		else value = 1;             /*value = 1 for dominations*/
	}
	else
	{
		m = 0;
		n = 0;
		while (m < nfunc && fit1[m] >= fit2[m])
		{
			if ((fit1[m] - fit2[m]) < EPS) n++;
			m++;
		}
		if (m == nfunc)
		{
			if (n != nfunc)
				value = 2;                       /*value =  2 for dominated */
			else value = 3;
		}
		else value = 3;                   /*value = 3 for incomparable*/
	}

	return value;
}
void ranking(population *pop_ptr)
{
	int i, j, k,       /*counters*/
		rnk,           /*rank*/
		val,           /*value obtained after comparing two individuals*/
		nondom,        /*no of non dominated members*/
		maxrank1,      /*Max rank of the population*/
		rankarr[250], /*Array storing the individual number at a rank*/
		q;


	double *ptr1, *ptr2;

	/*------------------------------* RANKING *------------------------------*/

	/*Initializing the ranks to zero*/
	rnk = 0;

	nondom = 0;
	maxrank1 = 0;

	/*Initializing all the flags to 2*/

	for (j = 0; j < popsize; j++)
	{
		pop_ptr->ind[j].flag = 2;
	}

	q = 0;

	for (k = 0; k < popsize; k++, q = 0)
	{
		for (j = 0; j < popsize; j++)
		{
			if (pop_ptr->ind[j].flag != 1)break;
			/*Break if all the individuals are assigned a rank*/
		}
		if (j == popsize)
			break;

		rnk = rnk + 1;

		for (j = 0; j < popsize; j++)
		{
			if (pop_ptr->ind[j].flag == 0) pop_ptr->ind[j].flag = 2;
			/*Set the flag of dominated individuals to 2*/
		}

		for (i = 0; i < popsize; i++)
		{
			/*Select an individual which rank to be assigned*/

			pop_ptr->ind_ptr = &(pop_ptr->ind[i]);

			if (pop_ptr->ind_ptr->flag != 1 && pop_ptr->ind_ptr->flag != 0)
			{
				ptr1 = &(pop_ptr->ind_ptr->fitness[0]);

				for (j = 0; j < popsize; j++)
				{

					/*Select the other individual which has not got a rank*/
					if (i != j)
					{
						if (pop_ptr->ind[j].flag != 1)
						{
							pop_ptr->ind_ptr = &(pop_ptr->ind[j]);
							ptr2 = &(pop_ptr->ind_ptr->fitness[0]);

							/*Compare the two individuals for fitness*/
							val = indcmp(ptr1, ptr2);

							/*VAL = 2 for dominated individual which rank to be given*/
							/*VAL = 1 for dominating individual which rank to be given*/

							/*VAL = 3 for non comparable individuals*/

							if (val == 2)
							{
								pop_ptr->ind[i].flag = 0;/* individual 1 is dominated */
								break;
							}

							if (val == 1)
							{
								pop_ptr->ind[j].flag = 0;/* individual 2 is dominated */
							}

							if (val == 3)
							{
								nondom++;/* individual 1 & 2 are non dominated */
								if (pop_ptr->ind[j].flag != 0)
									pop_ptr->ind[j].flag = 3;
							}

						}   /*if loop ends*/
					}       /* i != j loop ends*/
				}           /*loop over j ends*/
				if (j == popsize)
				{

					/*Assign the rank and set the flag*/
					pop_ptr->ind[i].rank = rnk;
					pop_ptr->ind[i].flag = 1;
					rankarr[q] = i;
					q++;
				}
			}       /*Loop over flag check ends*/
		}           /*Loop over i ends */
		pop_ptr->rankno[rnk - 1] = q;
	}
	maxrank1 = rnk;


	/* Find Max Rank of the population    */
	for (i = 0; i < popsize; i++)
	{
		rnk = pop_ptr->ind[i].rank;

		if (rnk > maxrank1)
			maxrank1 = rnk;

	}

	pop_ptr->maxrank = maxrank1;

	return;
}
double cal_reliab(int ind_module, double m_res){

	double rel = 0, rel1 = 0;

	rel = para_module[ind_module][0] * para_module[ind_module][1] * exp(-para_module[ind_module][1] * m_res);
	rel1 = exp(-rel*MISSION_TIME);

	return rel1;
}
double cal_reliabir1(int ind_module, double m_res){

	double rel = 0, rel1 = 0;

	rel = para_module3[ind_module][0] * para_module3[ind_module][1] * exp(-para_module3[ind_module][1] * m_res);
	rel1 = exp(-rel*MISSION_TIME);

	return rel1;
}
double cal_reliabir2(int ind_module, double m_res){

	double rel = 0, rel1 = 0;

	rel = para_module4[ind_module][0] * para_module4[ind_module][1] * exp(-para_module4[ind_module][1] * m_res);
	rel1 = exp(-rel*MISSION_TIME1);

	return rel1;
}
double cal_reliabir3(int ind_module, double m_res){

	double rel = 0, rel1 = 0;

	rel = para_module5[ind_module][0] * para_module5[ind_module][1] * exp(-para_module5[ind_module][1] * m_res);
	rel1 = exp(-rel*MISSION_TIME2);

	return rel1;
}
double cal_cost(int ind_module, double m_res){

	double cost = 0;
	cost = para_module3[ind_module][2] * exp(para_module3[ind_module][3] * cal_reliabir1(ind_module, m_res) - para_module3[ind_module][4]);

	return cost;
}
double cal_cost1(int ind_module, double m_res){

	double cost = 0;
	cost = para_module4[ind_module][2] * exp(para_module4[ind_module][3] * cal_reliabir2(ind_module, m_res) - para_module4[ind_module][4]);

	return cost;
}
double cal_cost2(int ind_module, double m_res){

	double cost = 0;
	cost = para_module5[ind_module][2] * exp(para_module5[ind_module][3] * cal_reliabir3(ind_module, m_res) - para_module5[ind_module][4]);

	return cost;
}
double cal_reliab1(int ind_module, double m_res){

	double rel = 0, rel1 = 0;

	rel = para_module1[ind_module][0] * para_module1[ind_module][1] * exp(-para_module1[ind_module][1] * m_res);
	rel1 = exp(-rel*MISSION_TIME1);

	return rel1;
}
double cal_reliab2(int ind_module, double m_res){

	double rel = 0, rel1 = 0;

	rel = para_module2[ind_module][0] * para_module2[ind_module][1] * exp(-para_module2[ind_module][1] * m_res);
	rel1 = exp(-rel*MISSION_TIME2);

	return rel1;
}
void func(population *pop_ptr)
{
	double *realx_ptr, /*Pointer to the array of x values*/
		*fitn_ptr,      /*Pointer to the array of fitness function*/
		x[2 * 10],     /* problem variables */
		f[maxfun];     /*array of fitness values*/

	int i, j, k, p;

	double cstr = 0.0;



	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

	/*Initializing the max rank to zero*/
	pop_ptr->maxrank = 0;
	for (i = 0; i < popsize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		realx_ptr = &(pop_ptr->ind_ptr->xreal[0]);


		for (j = 0; j < nvar; j++)
		{ // Real-coded variables 
			x[j] = *realx_ptr++;

			if (x[j] < 0.0){
				printf_s("error in x[j] < 0.0\n");
				getchar();
				exit(1);
			}
		}

		fitn_ptr = &(pop_ptr->ind_ptr->fitness[0]);


		/*First fitness function */
		/* calculate the reliability of each subsystem*/
		double sum1 = 0.0, sum2, sum3 = 0.0;
		j = 0;
		sum2 = 1.0;
		for (k = 0; k<nsystem; k++)
		{
			sum1 = 1.0;
			for (p = 0; p<no_subsys[k]; p++)
			{
				sum1 = sum1*(1.0 - cal_reliab(j, x[j]));
				sum3 += x[j];
				j++;
			}
			sum2 = sum2*(1.0 - sum1);

		}

		f[0] = 1 - sum2;/*here we use 1-sum2 to minimize f[0]*/
		f[1] = sum3;
		/******************************************************************/
		cstr = time_stage - sum3;/*if cstr[1]<0.0, we have sum3>time_stage*/
		if (cstr<0.0)
		{
			printf_s("error in the upper bound constraint\n");

			exit(1);
		}
			

		for (k = 0; k < nfunc; k++)
		{
			*fitn_ptr++ = f[k];
		}

	}

	/*---------------------------* RANKING *------------------------------*/
	ranking(pop_ptr);

}
void funcir(population *pop_ptr)
{
	double *realx_ptr, /*Pointer to the array of x values*/
		*fitn_ptr,      /*Pointer to the array of fitness function*/
		x[2 * maxvar],     /* problem variables */
		f[maxfun];     /*array of fitness values*/

	int i, j, k, p;

	double cstr = 0.0;

	double sum1 = 0, sum2 = 0;//added by Guofu Zhang

	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

	/*Initializing the max rank to zero*/
	pop_ptr->maxrank = 0;
	for (i = 0; i < popsize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		realx_ptr = &(pop_ptr->ind_ptr->xreal[0]);


		for (j = 0; j < nvar; j++)
		{ // Real-coded variables 
			x[j] = *realx_ptr++;

			if (x[j] < 0.0){
				printf_s("error in x[j] < 0.0\n");
				exit(1);
			}
		}

		fitn_ptr = &(pop_ptr->ind_ptr->fitness[0]);


		/*First fitness function */
		/* calculate the reliability of each subsystem*/
		j = 0;
		sum2 = 1.0;
		for (k = 0; k<MAX_SYSTEM; k++)
		{
			sum1 = 1.0;
			for (p = 0; p<no_subsys[k]; p++)
			{
				sum1 = sum1*(1.0 - cal_reliabir1(j, x[j]));
				j++;
			}
			sum2 = sum2*(1.0 - sum1);
		}

		f[0] = 1 - sum2;/*here we use 1-sum2 to minimize f[0]*/

		// Second and Third Fitness Functions
		sum1 = 0;
		sum2 = 0;

		for (j = 0; j < nvar; j++)
		{
			sum1 = sum1 + cal_cost(j, x[j]);
			sum2 = sum2 + x[j];
		}
		f[1] = sum1;
		f[2] = sum2;

		/******************************************************************/
		cstr = time_stage - sum2;/*if cstr[1]<0.0, we have sum3>time_stage*/
		if (cstr<0.0)
		{
			printf_s("error in the upper bound constraint\n");
			exit(1);
		}

		for (k = 0; k < nfunc; k++)
		{
			*fitn_ptr++ = f[k];
		}

	}

	/*---------------------------* RANKING *------------------------------*/
	ranking(pop_ptr);

}
void funcir1(population *pop_ptr)
{
	double *realx_ptr, /*Pointer to the array of x values*/
		*fitn_ptr,      /*Pointer to the array of fitness function*/
		x[2 * maxvar1],     /* problem variables */
		f[maxfun];     /*array of fitness values*/

	int i, j, k, p;

	double cstr = 0.0;

	double sum1 = 0, sum2 = 0;//added by Guofu Zhang

	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

	/*Initializing the max rank to zero*/
	pop_ptr->maxrank = 0;
	for (i = 0; i < popsize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		realx_ptr = &(pop_ptr->ind_ptr->xreal[0]);


		for (j = 0; j < nvar; j++)
		{ // Real-coded variables 
			x[j] = *realx_ptr++;

			if (x[j] < 0.0){
				printf_s("error in x[j] < 0.0\n");
				exit(1);
			}
		}

		fitn_ptr = &(pop_ptr->ind_ptr->fitness[0]);


		/*First fitness function */
		/* calculate the reliability of each subsystem*/
		j = 0;
		sum2 = 1.0;
		for (k = 0; k<MAX_SYSTEM1; k++)
		{
			sum1 = 1.0;
			for (p = 0; p<no_subsys1[k]; p++)
			{
				sum1 = sum1*(1.0 - cal_reliabir2(j, x[j]));
				j++;
			}
			sum2 = sum2*(1.0 - sum1);
		}

		f[0] = 1 - sum2;/*here we use 1-sum2 to minimize f[0]*/

		// Second and Third Fitness Functions
		sum1 = 0;
		sum2 = 0;

		for (j = 0; j < nvar; j++)
		{
			sum1 = sum1 + cal_cost1(j, x[j]);
			sum2 = sum2 + x[j];
		}
		f[1] = sum1;
		f[2] = sum2;

		/******************************************************************/
		cstr = time_stage - sum2;/*if cstr[1]<0.0, we have sum3>time_stage*/
		if (cstr<0.0)
		{
			printf_s("error in the upper bound constraint\n");
			exit(1);
		}

		for (k = 0; k < nfunc; k++)
		{
			*fitn_ptr++ = f[k];
		}

	}

	/*---------------------------* RANKING *------------------------------*/
	ranking(pop_ptr);

}
void funcir2(population *pop_ptr)
{
	double *realx_ptr, /*Pointer to the array of x values*/
		*fitn_ptr,      /*Pointer to the array of fitness function*/
		x[2 * maxvar2],     /* problem variables */
		f[maxfun];     /*array of fitness values*/

	int i, j, k, p;

	double cstr = 0.0;

	double sum1 = 0, sum2 = 0;//added by Guofu Zhang

	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

	/*Initializing the max rank to zero*/
	pop_ptr->maxrank = 0;
	for (i = 0; i < popsize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		realx_ptr = &(pop_ptr->ind_ptr->xreal[0]);


		for (j = 0; j < nvar; j++)
		{ // Real-coded variables 
			x[j] = *realx_ptr++;

			if (x[j] < 0.0){
				printf_s("error in x[j] < 0.0\n");
				exit(1);
			}
		}

		fitn_ptr = &(pop_ptr->ind_ptr->fitness[0]);


		/*First fitness function */
		/* calculate the reliability of each subsystem*/
		j = 0;
		sum2 = 1.0;
		for (k = 0; k<MAX_SYSTEM2; k++)
		{
			sum1 = 1.0;
			for (p = 0; p<no_subsys2[k]; p++)
			{
				sum1 = sum1*(1.0 - cal_reliabir3(j, x[j]));
				j++;
			}
			sum2 = sum2*(1.0 - sum1);
		}

		f[0] = 1 - sum2;/*here we use 1-sum2 to minimize f[0]*/

		// Second and Third Fitness Functions
		sum1 = 0;
		sum2 = 0;

		for (j = 0; j < nvar; j++)
		{
			sum1 = sum1 + cal_cost2(j, x[j]);
			sum2 = sum2 + x[j];
		}
		f[1] = sum1;
		f[2] = sum2;

		/******************************************************************/
		cstr = time_stage - sum2;/*if cstr[1]<0.0, we have sum3>time_stage*/
		if (cstr<0.0)
		{
			printf_s("error in the upper bound constraint\n");
			exit(1);
		}

		for (k = 0; k < nfunc; k++)
		{
			*fitn_ptr++ = f[k];
		}

	}

	/*---------------------------* RANKING *------------------------------*/
	ranking(pop_ptr);

}
void func1(population *pop_ptr)
{
	double *realx_ptr, /*Pointer to the array of x values*/
		*fitn_ptr,      /*Pointer to the array of fitness function*/
		x[2 * 20],     /* problem variables */
		f[maxfun];     /*array of fitness values*/

	int i, j, k, p;

	double cstr = 0.0;



	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

	/*Initializing the max rank to zero*/
	pop_ptr->maxrank = 0;
	for (i = 0; i < popsize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		realx_ptr = &(pop_ptr->ind_ptr->xreal[0]);


		for (j = 0; j < nvar; j++)
		{ // Real-coded variables 
			x[j] = *realx_ptr++;

			if (x[j] < 0.0){
				printf_s("error in x[j] < 0.0\n");
				getchar();
				exit(1);
			}
		}

		fitn_ptr = &(pop_ptr->ind_ptr->fitness[0]);


		/*First fitness function */
		/* calculate the reliability of each subsystem*/
		double sum1 = 0.0, sum2, sum3 = 0.0;
		j = 0;
		sum2 = 1.0;
		for (k = 0; k<nsystem; k++)
		{
			sum1 = 1.0;
			for (p = 0; p<no_subsys1[k]; p++)
			{
				sum1 = sum1*(1.0 - cal_reliab1(j, x[j]));
				sum3 += x[j];
				j++;
			}
			sum2 = sum2*(1.0 - sum1);

		}

		f[0] = 1 - sum2;/*here we use 1-sum2 to minimize f[0]*/
		f[1] = sum3;
		/******************************************************************/
		cstr = time_stage - sum3;/*if cstr[1]<0.0, we have sum3>time_stage*/
		if (cstr<0.0)
		{
			printf_s("error in the upper bound constraint\n");

			exit(1);
		}


		for (k = 0; k < nfunc; k++)
		{
			*fitn_ptr++ = f[k];
		}

	}

	/*---------------------------* RANKING *------------------------------*/
	ranking(pop_ptr);

}
void func2(population *pop_ptr)
{
	double *realx_ptr, /*Pointer to the array of x values*/
		*fitn_ptr,      /*Pointer to the array of fitness function*/
		x[2 * 20],     /* problem variables */
		f[maxfun];     /*array of fitness values*/

	int i, j, k, p;

	double cstr = 0.0;



	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

	/*Initializing the max rank to zero*/
	pop_ptr->maxrank = 0;
	for (i = 0; i < popsize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		realx_ptr = &(pop_ptr->ind_ptr->xreal[0]);


		for (j = 0; j < nvar; j++)
		{ // Real-coded variables 
			x[j] = *realx_ptr++;

			if (x[j] < 0.0){
				printf_s("error in x[j] < 0.0\n");
				getchar();
				exit(1);
			}
		}

		fitn_ptr = &(pop_ptr->ind_ptr->fitness[0]);


		/*First fitness function */
		/* calculate the reliability of each subsystem*/
		double sum1 = 0.0, sum2, sum3 = 0.0;
		j = 0;
		sum2 = 1.0;
		for (k = 0; k<nsystem; k++)
		{
			sum1 = 1.0;
			for (p = 0; p<no_subsys2[k]; p++)
			{
				sum1 = sum1*(1.0 - cal_reliab2(j, x[j]));
				sum3 += x[j];
				j++;
			}
			sum2 = sum2*(1.0 - sum1);

		}

		f[0] = 1 - sum2;/*here we use 1-sum2 to minimize f[0]*/
		f[1] = sum3;
		/******************************************************************/
		cstr = time_stage - sum3;/*if cstr[1]<0.0, we have sum3>time_stage*/
		if (cstr<0.0)
		{
			printf_s("error in the upper bound constraint\n");

			exit(1);
		}


		for (k = 0; k < nfunc; k++)
		{
			*fitn_ptr++ = f[k];
		}

	}

	/*---------------------------* RANKING *------------------------------*/
	ranking(pop_ptr);

}
void realcross(population *new_pop_ptr, population *mate_pop_ptr)
{
	int a, b, c;
	double m;
	individual *parent0, *parent1, *parent2,
		*current;

	for (int i = 0; i < popsize; i++){

		a = rand() % popsize;
		/*b = rand() % popsize;
		c = rand() % popsize;*/
		do {
			b = rand() % popsize;
		} while (a == b);
		do {
			c = rand() % popsize;
		} while (a == c || c == b);

		current = &(mate_pop_ptr->ind[i]);
		parent0 = &(mate_pop_ptr->ind[a]);
		parent1 = &(mate_pop_ptr->ind[b]);
		parent2 = &(mate_pop_ptr->ind[c]);
		int jrand1 = rnd(0, nvar - 1);
		double sumk = 0.0;
		double sumofmut = 0.0;
		double r = 0.0;
		bool *flag_c = new bool[nvar];
		for (int j = 0; j < nvar; j++)
		{
			flag_c[j] = false;
			m = randomperc();
			double value = 0.0;
			if (m < CR || j == jrand1){
				value = parent2->xreal[j] + F*(parent0->xreal[j] - parent1->xreal[j]);
				flag_c[j] = true;

			}
			else
			{
				value = current->xreal[j];

			}
			if (value < 0.0) {
				value = RandomDouble(0, current->xreal[j]);
				flag_c[j] = true;
			}
			if (value > time_stage) {
				value = rndreal(current->xreal[j], time_stage);
				flag_c[j] = true;
			}
			new_pop_ptr->ind[i].xreal[j] = value;
			if (flag_c[j] == true)
			{
				sumofmut += value;
			}
			sumk = sumk + new_pop_ptr->ind[i].xreal[j];
		}

		if (sumk>time_stage)
		{
			double timeofmut = time_stage - (sumk - sumofmut);

			for (int j = 0; j < nvar; j++)
			{
				if (flag_c[j] == true) {
					r = randomperc();
					new_pop_ptr->ind[i].xreal[j] = new_pop_ptr->ind[i].xreal[j] * r * (timeofmut / sumofmut);
				}

			}

		}
		delete[]flag_c;

	}

}
void realcrossir(population *new_pop_ptr, population *mate_pop_ptr)
{
	int i, j, y, n;
	double rnd1, rnd3, par1, par2, chld1, chld2, betaq, beta, alpha;
	double y1, y2, yu, yl, expp;

	int k, q;
	double rnd2;
	double sum_chld1, sum_chld2, sum_cross1, sum_cross2, temp1, temp2;
	int no_cross, pos_cross[maxvar2];//save the crossover points in the encoding
	double gama;//the scale factor for controlling the enlargement and shrink of gene values

	y = 0; n = 0;
	for (i = 0; i < popsize / 2; i++)
	{
		rnd1 = randomperc();

		/*Check Whether the cross-over to be performed*/
		if (rnd1 <= pcross)
		{
			sum_chld1 = 0;
			sum_chld2 = 0;
			sum_cross1 = 0;
			sum_cross2 = 0;

			no_cross = 0;
			for (j = 0; j < nvar; j++)
				pos_cross[j] = -1;

			q = -1;

			/*Loop over no of variables*/
			for (j = 0; j < nvar; j++)
			{
				/*Selected Two Parents*/
				par1 = mate_pop_ptr->ind[y].xreal[j];
				par2 = mate_pop_ptr->ind[y + 1].xreal[j];

				yl = 0.0;
				yu = time_stage;

				rnd3 = randomperc();

				/* Check whether variable is selected or not*/
				if (rnd3 <= 0.5)
				{
					pos_cross[no_cross] = j;
					no_cross++;
					/*Variable selected*/

					if (fabs(par1 - par2) > EPS) // changed by Deb (31/10/01)
					{
						if (par2 > par1)
						{
							y2 = par2;
							y1 = par1;
						}
						else
						{
							y2 = par1;
							y1 = par2;
						}

						/*Find beta value*/
						if ((y1 - yl) > (yu - y2))
						{
							beta = 1 + (2 * (yu - y2) / (y2 - y1));
							//printf("beta = %f\n",beta);
						}
						else
						{
							beta = 1 + (2 * (y1 - yl) / (y2 - y1));
							//printf("beta = %f\n",beta);
						}

						/*Find alpha*/
						expp = di + 1.0;

						beta = 1.0 / beta;

						alpha = 2.0 - pow(beta, expp);

						if (alpha <0.0)
						{
							printf("ERRRROR %f %d %d %f %f\n", alpha, y, n, par1, par2);
							exit(-1);
						}

						rnd2 = randomperc();

						if (rnd2 <= 1.0 / alpha)
						{
							alpha = alpha*rnd2;
							expp = 1.0 / (di + 1.0);
							betaq = pow(alpha, expp);
						}
						else
						{
							alpha = alpha*rnd2;
							alpha = 1.0 / (2.0 - alpha);
							expp = 1.0 / (di + 1.0);
							if (alpha <0.0)
							{
								printf("ERRRORRR \n");
								exit(-1);
							}
							betaq = pow(alpha, expp);
						}

						/*Generating two children*/
						chld1 = 0.5*((y1 + y2) - betaq*(y2 - y1));
						chld2 = 0.5*((y1 + y2) + betaq*(y2 - y1));

					}
					else
					{

						betaq = 1.0;
						y1 = par1; y2 = par2;

						/*Generation two children*/
						chld1 = 0.5*((y1 + y2) - betaq*(y2 - y1));
						chld2 = 0.5*((y1 + y2) + betaq*(y2 - y1));

					}

					/*Repair encoding*/
					if (chld1 < 0.0 || chld2 > time_stage)//This means that chld1 is reduced, then it is certain that chld2 is enlarged
					{/*Select a minimum interval to generate a child to ensure the other child obeys the upper or lower constraint*/

						if (y1 <= (time_stage - y2))
						{
							rnd2 = randomperc();
							chld1 = rnd2 * y1;//generate a value over the range (0,y1) to ensure chld1 obeys the lower constraint
							chld2 = y1 + y2 - chld1;
						}
						else{
							rnd2 = randomperc();
							chld2 = rnd2 * (time_stage - y2) + y2;//generate a value over the range (y2,yu) to ensure chld2 obeys the upper constraint
							chld1 = y1 + y2 - chld2;
						}
					}

					if (chld1 >time_stage)
					{
						printf("ERRRORRR for chld1\n");
						exit(-1);
						//	rnd2 = randomperc();
						//	chld1 = rnd2 * (yu - y1) + y1; 
						//	chld2 = y1 + y2 - chld1;

					}
					sum_cross2 = sum_cross2 + chld2;//the sum of variables in the crossover points
					sum_cross1 = sum_cross1 + chld1;//the sum of variables in the crossover points

				}
				else
				{

					/*Copying the children to parents*/
					chld1 = par1;
					chld2 = par2;
				}
				new_pop_ptr->ind[n].xreal[j] = chld1;
				new_pop_ptr->ind[n + 1].xreal[j] = chld2;

				sum_chld2 = sum_chld2 + chld2;//the sum of all variables
				sum_chld1 = sum_chld1 + chld1;
			}

			/*Repair encoding*/
			if (sum_chld1>time_stage)/*chld1 violates the upper constraint,then it is certain than sum_chld2<yu2 based on the average property*/
			{

				if (sum_cross1 - time_stage + sum_chld2 <= 0.0)
					gama = 0;
				else{
					if (time_stage - sum_chld1 + sum_cross1 == 0)
						gama = 1.0;
					else
						gama = (sum_cross1 - time_stage + sum_chld2) / (time_stage - sum_chld1 + sum_cross1);
					if (gama > 1.0)
						gama = 1.0;	//if (time_stage - sum_chld1 + sum_cross1)==0 							
				}
				for (k = 0; k < no_cross; k++)//Only repair the mutation genes to maintain and inherit the parent's information
				{
					q = pos_cross[k];

					temp1 = new_pop_ptr->ind[n].xreal[q];//chld1
					temp2 = new_pop_ptr->ind[n + 1].xreal[q];//chld2

					rnd2 = randomperc();
					new_pop_ptr->ind[n].xreal[q] = temp1 * (rnd2 * (1.0 - gama) + gama)*((time_stage - sum_chld1 + sum_cross1) / sum_cross1);
					new_pop_ptr->ind[n + 1].xreal[q] = temp1 + temp2 - new_pop_ptr->ind[n].xreal[q];
				}


			}

			if (sum_chld2>time_stage)/*chld2 violates the upper constraint,then it is certain than sum_chld1<yu1 based on the average property*/
			{

				if (sum_cross2 - time_stage + sum_chld1 <= 0.0)
					gama = 0;
				else{
					if (time_stage - sum_chld2 + sum_cross2 == 0)
						gama = 1.0;
					else
						gama = (sum_cross2 - time_stage + sum_chld1) / (time_stage - sum_chld2 + sum_cross2);

					if (gama > 1.0)
						gama = 1.0;// if (yu - sum_chld2 + sum_cross2)==0							
				}

				for (k = 0; k < no_cross; k++)//Only repair the mutation genes to maintain and inherit the parent's information
				{
					q = pos_cross[k];

					temp1 = new_pop_ptr->ind[n + 1].xreal[q];//chld2
					temp2 = new_pop_ptr->ind[n].xreal[q];//chld1

					rnd2 = randomperc();
					new_pop_ptr->ind[n + 1].xreal[q] = temp1 * (rnd2 * (1.0 - gama) + gama)*((time_stage - sum_chld2 + sum_cross2) / sum_cross2);
					new_pop_ptr->ind[n].xreal[q] = temp1 + temp2 - new_pop_ptr->ind[n + 1].xreal[q];
				}

			}

		}
		else
		{
			for (j = 0; j < nvar; j++)
			{
				par1 = mate_pop_ptr->ind[y].xreal[j];
				par2 = mate_pop_ptr->ind[y + 1].xreal[j];
				chld1 = par1;
				chld2 = par2;
				new_pop_ptr->ind[n].xreal[j] = chld1;
				new_pop_ptr->ind[n + 1].xreal[j] = chld2;
			}
		}
		n = n + 2; y = y + 2;
	}

}
void real_mutate(population *new_pop_ptr)
{
	int i, j;

	int k, q;

	double rnd1, rnd3, delta, indi, deltaq;

	double y, yl, yu, val, xy;

	double rnd2, chld, sum, sum_muta;
	int no_muta, pos_muta[maxvar];//save the mutation points in the encoding


	for (j = 0; j < popsize; j++)
	{
		sum = 0;
		sum_muta = 0;

		no_muta = 0;
		for (i = 0; i < nvar; i++)
			pos_muta[i] = -1;

		q = -1;

		for (i = 0; i < nvar; i++)
		{
			rnd1 = randomperc();

			/*For each variable find whether to do mutation or not*/
			if (rnd1 <= pmut_r)
			{
				pos_muta[no_muta] = i;//save the positions of mutation genes
				no_muta++;

				y = new_pop_ptr->ind[j].xreal[i];
				yl = 0.0;
				yu = time_stage;


				/*Calculate delta*/

				if ((y - yl) <(yu - y))
					delta = (y - yl) / (yu - yl);
				else
					delta = (yu - y) / (yu - yl);

				rnd3 = randomperc();

				indi = 1.0 / (dim + 1.0);

				if (rnd3 <= 0.5)
				{
					xy = 1.0 - delta;
					val = 2 * rnd3 + (1 - 2 * rnd3)*(pow(xy, (dim + 1)));
					deltaq = pow(val, indi) - 1.0;
				}
				else
				{
					xy = 1.0 - delta;
					val = 2.0*(1.0 - rnd3) + 2.0*(rnd3 - 0.5)*(pow(xy, (dim + 1)));
					deltaq = 1.0 - (pow(val, indi));
				}

				/*Change the value for the parent */
				chld = y + deltaq * (yu - yl);

				/*Repair encoding*/
				if (chld <0.0)
				{
					rnd2 = randomperc();
					chld = rnd2*y;

				}

				if (chld > time_stage)
				{
					rnd2 = randomperc();
					chld = rnd2*(time_stage - y) + y;
				}

				new_pop_ptr->ind[j].xreal[i] = chld;

				sum_muta = sum_muta + new_pop_ptr->ind[j].xreal[i];//the sum of variables in the mutation points
			}//end of mutation
			sum = sum + new_pop_ptr->ind[j].xreal[i];//the sum of all variables
		}//end of i

		/*Repair encoding*/
		if (sum>time_stage)
		{

			for (k = 0; k < no_muta; k++)//Only repair the mutation genes to maintain and inherit the parent's information
			{
				q = pos_muta[k];
				rnd2 = randomperc();
				new_pop_ptr->ind[j].xreal[q] = new_pop_ptr->ind[j].xreal[q] * rnd2 * ((time_stage - sum + sum_muta) / sum_muta);
			}

		}

	}

}
int indcmp1(double *ptr1, double *ptr2)
{
	double fit1[maxfun1], fit2[maxfun1];
	int i, value, m, n;
	for (i = 0; i < nfunc; i++)
	{
		fit1[i] = *ptr1++;
		fit2[i] = *ptr2++;
	}
	m = 0; n = 0;
	while (m < nfunc && fit1[m] <= fit2[m])
	{
		if ((fit2[m] - fit1[m]) < EPS) n++;
		m++;
	}
	if (m == nfunc)
	{
		if (n == nfunc) value = 3;
		else value = 1;              //value = 1 for dominating
	}
	else
	{
		m = 0; n = 0;
		while (m < nfunc && fit1[m] >= fit2[m])
		{
			if ((fit1[m] - fit2[m]) < EPS) n++;
			m++;
		}
		if (m == nfunc)
		{
			if (n != nfunc)
				value = 2;          //value =  2 for dominated 
			else value = 3;
		}
		else value = 3;             //value = 3 for incomparable
	}
	return value;
}
void gsort(int rnk, int sel)//sort the dummy fitness arrays 
{
	int i, j, a, q;
	double array[2 * maxpop2][2], temp, temp1;

	q = globalpop.rankno[rnk - 1];

	for (i = 0; i < q; i++)
	{
		if ((popn == 0||zqs==50) && (i >= 100))
			rnk += 1;
		if ((popn == 1||zqs==50) && (i >= 200))
			rnk += 1;
		array[i][0] = globalpop.rankar[rnk - 1][i];
		a = globalpop.rankar[rnk - 1][i];
		array[i][1] = globalpop.cub_len[a];
	}
	for (i = 0; i < q; i++)
	{
		for (j = i + 1; j < q; j++)
		{
			if (array[i][1] < array[j][1])
			{
				temp = array[i][1];
				temp1 = array[i][0];
				array[i][1] = array[j][1];
				array[i][0] = array[j][0];

				array[j][1] = temp;
				array[j][0] = temp1;
			}
		}
	}

	for (i = 0; i < sel; i++)
	{
		a = (int)array[i][0];
		globalpop.flag[a] = 1;
	}

}
void gshare(int rnk)
{
	double length[2 * maxpop2][2], max;
	int i, j, m1, a;
	double min, Diff;  // Added 18.08.2003

	m1 = globalpop.rankno[rnk - 1];

	for (j = 0; j < nfunc; j++)
	{
		for (i = 0; i < m1; i++)
		{
			fpara1[i][0] = 0;
			fpara1[i][1] = 0;
		}

		for (i = 0; i < m1; i++)
		{
			if ((popn == 0||zqs==50) && (i >= 100))
				rnk += 1;
			if ((popn == 1||zqs==100) && (i >= 200))
				rnk += 1;
			a = globalpop.rankar[rnk - 1][i];
			fpara1[i][0] = (double)a;
			fpara1[i][1] = globalpop.fitness[a][j];
		}

		sort(m1); /*Sort the arrays in ascending order of the fitness*/

		max = fpara1[m1 - 1][1];
		min = fpara1[0][1];  // Added 18.08.2003
		Diff = max - min;      // Added 18.08.2003 and 5 subsequent lines
		if (Diff < 0.0)
		{
			printf("Something wrong in keepaliven.h\n");
			exit(1);
		}
		for (i = 0; i < m1; i++)
		{
			if (i == 0 || i == (m1 - 1))
			{
				length[i][0] = fpara1[i][0];
				length[i][1] = 100 * max;
			}
			else
			{
				length[i][0] = fpara1[i][0];
				length[i][1] = fabs(fpara1[i + 1][1] - fpara1[i - 1][1]) / Diff; // crowding distances are normalized 18.08.2003
			}
		}
		for (i = 0; i < m1; i++)
		{
			a = (int)length[i][0];
			globalpop.cub_len[a] += length[i][1];
		}
	}

};

void grank(int gen, int y)
{
	int i, j, k, rnk, val, nondom, popsize1, gflg[2 * maxpop2], q;
	double *ptr1, *ptr2;

	/*----------------------------* RANKING *---------------------------------*/
	rnk = 0;
	nondom = 0;

	popsize1 = y + popsize;

	for (i = 0; i < popsize1; i++)
	{
		gflg[i] = 2;
	}

	for (k = 0; k < popsize1; k++)
	{
		q = 0;
		for (j = 0; j < popsize1; j++)
		{
			if (gflg[j] != 1) break;
		}
		if (j == popsize1) break;
		rnk = rnk + 1;
		for (j = 0; j < popsize1; j++)
		{
			if (gflg[j] == 0) gflg[j] = 2;
		}
		for (i = 0; i < popsize1; i++)
		{
			if (gflg[i] != 1 && gflg[i] != 0)
			{
				ptr1 = &(global_pop_ptr->fitness[i][0]);
				for (j = 0; j < popsize1; j++)
				{
					if (i != j)
					{
						if (gflg[j] != 1)
						{
							ptr2 = &(global_pop_ptr->fitness[j][0]);
							val = indcmp1(ptr1, ptr2);
							if (val == 2)
							{
								gflg[i] = 0;/* individual 1 is dominated */
								break;
							}
							if (val == 1)
							{
								gflg[j] = 0;/* individual 2 is dominated */
							}
							if (val == 3)
							{
								nondom++;/* individual 1 & 2 are non dominated */
								if (gflg[j] != 0)gflg[j] = 3;
							}
						}
					}
				}
				if (j == popsize1)
				{
					if ((popn == 0||zqs==50) && (q >= 100))
						rnk += 1;
					if ((popn == 1||zqs==100) && (q >= 200))
						rnk += 1;
					global_pop_ptr->rank[i] = rnk;
					gflg[i] = 1;
					global_pop_ptr->rankar[rnk - 1][q] = i;
					q++;
				}
			}
		}
		global_pop_ptr->rankno[rnk - 1] = q;
	}
	global_pop_ptr->maxrank = rnk;


}
void grankir(int gen)
{
	int i, j, k, rnk, val, nondom, popsize1, gflg[2 * maxpop2], q;
	double *ptr1, *ptr2;

	/*----------------------------* RANKING *---------------------------------*/
	rnk = 0;
	nondom = 0;

	popsize1 = 2 * popsize;

	for (i = 0; i < popsize1; i++)
	{
		gflg[i] = 2;
	}

	for (k = 0; k < popsize1; k++)
	{
		q = 0;
		for (j = 0; j < popsize1; j++)
		{
			if (gflg[j] != 1) break;
		}
		if (j == popsize1) break;
		rnk = rnk + 1;
		for (j = 0; j < popsize1; j++)
		{
			if (gflg[j] == 0) gflg[j] = 2;
		}
		for (i = 0; i < popsize1; i++)
		{
			if (gflg[i] != 1 && gflg[i] != 0)
			{
				ptr1 = &(global_pop_ptr->fitness[i][0]);
				for (j = 0; j < popsize1; j++)
				{
					if (i != j)
					{
						if (gflg[j] != 1)
						{
							ptr2 = &(global_pop_ptr->fitness[j][0]);
							val = indcmp1(ptr1, ptr2);
							if (val == 2)
							{
								gflg[i] = 0;/* individual 1 is dominated */
								break;
							}
							if (val == 1)
							{
								gflg[j] = 0;/* individual 2 is dominated */
							}
							if (val == 3)
							{
								nondom++;/* individual 1 & 2 are non dominated */
								if (gflg[j] != 0)gflg[j] = 3;
							}
						}
					}
				}
				if (j == popsize1)
				{
					if ((popn == 0||zqs==50) && (q >= 100))
						rnk += 1;
					if ((popn == 1||zqs==100) && (q >= 200))
						rnk += 1;
					global_pop_ptr->rank[i] = rnk;
					gflg[i] = 1;
					global_pop_ptr->rankar[rnk - 1][q] = i;
					q++;
				}
			}
		}
		global_pop_ptr->rankno[rnk - 1] = q;
	}
	global_pop_ptr->maxrank = rnk;

	return;
}
void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen)
{
	int i, j, m, l, rec;

	int st, pool, poolf, sel;

	double *gene3_ptr, *gene4_ptr;

	/*Forming the global mating pool*/
	int rem = 0;
	for (i = 0; i < popsize; i++)
	{
		if (nvar > 0)
		{	/*For Real Coded GA x values are copied */
			if (indcmp1(pop1_ptr->ind[i].fitness, pop2_ptr->ind[i].fitness) == 1)
			{
				for (int k = 0; k < nvar; k++)
				{
					if ((sc ==0 ) && (k >= 10))
						i += 1;
					if ((sc == 1) && (k >= 14))
						i += 1;
					globalpop.xreal[i][k] = pop1_ptr->ind[i].xreal[k];
				}
				for (l = 0; l < nfunc; l++)
				{
					globalpop.fitness[i][l] = pop1_ptr->ind[i].fitness[l];

				}
			}
			else if (indcmp1(pop1_ptr->ind[i].fitness, pop2_ptr->ind[i].fitness) == 2)
			{
				for (int k = 0; k < nvar; k++)
				{
					if ((sc == 0) && (k >= 10))
						i += 1;
					if ((sc == 1) && (k >= 14))
						i += 1;
					globalpop.xreal[i][k] = pop2_ptr->ind[i].xreal[k];
				}
				for (l = 0; l < nfunc; l++)
				{

					globalpop.fitness[i][l] = pop2_ptr->ind[i].fitness[l];
				}

			}
			else if (indcmp1(pop1_ptr->ind[i].fitness, pop2_ptr->ind[i].fitness) == 3)
			{
				for (int k = 0; k < nvar; k++)
				{
					if ((sc == 0) && (k >= 10))
						i += 1;
					if ((sc == 1) && (k >= 14))
						i += 1;
					globalpop.xreal[i][k] = pop1_ptr->ind[i].xreal[k];
					globalpop.xreal[rem + popsize][k] = pop2_ptr->ind[i].xreal[k];

				}
				for (l = 0; l < nfunc; l++)
				{
					globalpop.fitness[i][l] = pop1_ptr->ind[i].fitness[l];
					globalpop.fitness[rem + popsize][l] = pop2_ptr->ind[i].fitness[l];
				}
				rem++;
			}

		}
		/*Fitness is copied to the global pool */


		/*Initial;ising the dummyfitness to zero */
		globalpop.cub_len[i] = 0;
		globalpop.cub_len[i + popsize] = 0;

	}


	global_pop_ptr = &(globalpop);

	/*Finding the global ranks */
	grank(gen, rem);

	m = globalpop.maxrank;

	/* Sharing the fitness to get the dummy fitness */
	for (i = 0; i < m; i++)
	{
		gshare(i + 1);
	}

	poolf = popsize;
	pool = 0;


	/*Initializing the flags of population to zero */
	for (i = 0; i < rem + popsize; i++)
	{
		globalpop.flag[i] = 0;
	}
	// decide which all solutions belong to the pop3 
	rec = 0;
	st = 0;
	for (i = 0; i < m; i++)
	{
		/*    Elitism Applied Here     */
		st = pool;
		pool += globalpop.rankno[i];

		if (pool <= popsize)
		{
			for (int k = 0; k < rem + popsize; k++)
			{
				if (globalpop.rank[k] == i + 1)
					globalpop.flag[k] = 1;
			}
			pop3_ptr->rankno[i] = globalpop.rankno[i];
		}
		else
		{
			sel = popsize - st;
			Lastrank = i + 1;
			pop3_ptr->rankno[i] = sel;
			gsort(i + 1, sel);
			break;
		}
	}

	int k = 0;
	for (i = 0, k = 0; i < rem + popsize && k < popsize; i++)
	{
		if (nvar > 0)
		{
			if (globalpop.flag[i] == 1)
			{
				gene3_ptr = &(globalpop.xreal[i][0]);
				pop3_ptr->ind_ptr = &(pop3_ptr->ind[k]);
				gene4_ptr = &(pop3_ptr->ind_ptr->xreal[0]);

				for (j = 0; j < nvar; j++)
				{
					*gene4_ptr++ = *gene3_ptr++;
				}
			}
		}
		if (globalpop.flag[i] == 1)
		{
			for (j = 0; j < nfunc; j++)
				pop3_ptr->ind[k].fitness[j] = globalpop.fitness[i][j];
			pop3_ptr->ind[k].cub_len = globalpop.cub_len[i];

			pop3_ptr->ind[k].rank = globalpop.rank[i];
			k++;  // increment the pop3 counter
		}
	}

	pop3_ptr->maxrank = Lastrank;

}
void keepaliveir(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen)
{
	int i, j, k, m, l, rec;

	int st, pool, poolf, sel;

	double *gene3_ptr, *gene4_ptr;

	/*Forming the global mating pool*/

	for (i = 0; i < popsize; i++)
	{
		if (nvar > 0)
		{	/*For Real Coded GA x values are copied */
			for (k = 0; k < nvar; k++)
			{
				globalpop.xreal[i][k] = pop1_ptr->ind[i].xreal[k];
				globalpop.xreal[i + popsize][k] = pop2_ptr->ind[i].xreal[k];
			}
		}

		/*Fitness is copied to the global pool */
		for (l = 0; l < nfunc; l++)
		{
			globalpop.fitness[i][l] = pop1_ptr->ind[i].fitness[l];
			globalpop.fitness[i + popsize][l] = pop2_ptr->ind[i].fitness[l];
		}

		/*Initial;ising the dummyfitness to zero */
		globalpop.cub_len[i] = 0;
		globalpop.cub_len[i + popsize] = 0;

	}


	global_pop_ptr = &(globalpop);

	/*Finding the global ranks */
	grankir(gen);

	m = globalpop.maxrank;

	/* Sharing the fitness to get the dummy fitness */
	for (i = 0; i < m; i++)
	{
		gshare(i + 1);
	}

	poolf = popsize;
	pool = 0;


	/*Initializing the flags of population to zero */
	for (i = 0; i < 2 * popsize; i++)
	{
		globalpop.flag[i] = 0;
	}
	// decide which all solutions belong to the pop3 
	rec = 0;
	st = 0;
	for (i = 0; i < m; i++)
	{
		/*    Elitism Applied Here     */
		st = pool;
		pool += globalpop.rankno[i];

		if (pool <= popsize)
		{
			for (k = 0; k < 2 * popsize; k++)
			{
				if (globalpop.rank[k] == i + 1)
					globalpop.flag[k] = 1;
			}
			pop3_ptr->rankno[i] = globalpop.rankno[i];
		}
		else
		{
			sel = popsize - st;
			Lastrank = i + 1;
			pop3_ptr->rankno[i] = sel;
			gsort(i + 1, sel);
			break;
		}
	}

	k = 0;
	for (i = 0, k = 0; i < 2 * popsize && k < popsize; i++)
	{
		if (nvar > 0)
		{
			if (globalpop.flag[i] == 1)
			{
				gene3_ptr = &(globalpop.xreal[i][0]);
				pop3_ptr->ind_ptr = &(pop3_ptr->ind[k]);
				gene4_ptr = &(pop3_ptr->ind_ptr->xreal[0]);

				for (j = 0; j < nvar; j++)
				{
					*gene4_ptr++ = *gene3_ptr++;
				}
			}
		}
		if (globalpop.flag[i] == 1)
		{
			for (j = 0; j < nfunc; j++)
				pop3_ptr->ind[k].fitness[j] = globalpop.fitness[i][j];
			pop3_ptr->ind[k].cub_len = globalpop.cub_len[i];

			pop3_ptr->ind[k].rank = globalpop.rank[i];
			k++;  // increment the pop3 counter
		}
	}

	pop3_ptr->maxrank = Lastrank;

}
void re_parameter(individual *ind, double &cost, int no)
{
	//CLEFTTOP::OnBnClickedButalog();
	int noerr[maxvar];
	int i = 0;
	for (int k = 0; k< nsystem; k++) {
		for (int j = 0; j < no_subsys[k]; j++) {
			noerr[i] = int(para_module[i][0] * err_detect[no][i]);
			/*noerr[i] = rnd(0, int(para_module[i][0]));*/
			double res_err[350];
			int on = 0;
			while (on < noerr[i]) {
				if (ind->xreal[i] < 0.1) { noerr[i] = 0; break; }
				/*double s = randomperc()* ind->xreal[i];*/
				double s = randomperc()*ind->xreal[i]; /*ind->fitness[1] /nsystem/ no_subsys[k];*/
				double pr = 1 - exp(-para_module[i][1] * s);
				double rand = randomperc();
				if (rand < pr) {
					res_err[on] = s;
					on++;

				}
			}
			for (int p = 0; p < noerr[i]; p++) {
				for (int q = p + 1; q < noerr[i]; q++) {
					if (res_err[p] > res_err[q]) {
						double tr = res_err[q];
						res_err[q] = res_err[p];
						res_err[p] = tr;
					}
				}
			}
			if (noerr[i] > 0) {
				double at = log(1.0 - noerr[i] / para_module[i][0]);
				/*para_module[i][1] = 0.0 - at / ind->xreal[i];*/
				para_module[i][1] = 0.0 - at / res_err[noerr[i] - 1];
			}
			para_module[i][0] = para_module[i][0] - noerr[i];
			i++;
		}
	}
	//for (int i = 0; i < nvar; i++) {
	//	/*noerr[i] = rnd(0,int(para_module[i][0]));*/
	//	noerr[i] =  int(para_module1[i][0]*tperiod[no-1]/(T_MAX_RESOURCE-tperiod[noperiods-1]));
	//	/*double *res_err = new double[noerr[i]];*/
	//	double res_err[350];
	//	int on = 0;
	//	while (on < noerr[i]) {
	//		/*if (ind->xreal[i] < 0.01) { noerr[i] = 0; break; }*/
	//		/*double s = randomperc()* ind->xreal[i];*/
	//		double s = randomperc()* tperiod[no-1]/nvar;
	//		double pr = 1 - exp(-para_module[i][1] * s);
	//		double rand = randomperc();
	//		if (rand < pr) {
	//			res_err[on] = s;
	//			on++;

	//		}
	//	}
	//	for (int j = 0; j < noerr[i]; j++) {
	//		for (int k = j + 1; k < noerr[i]; k++) {
	//			if (res_err[j] > res_err[k]) {
	//				double tr = res_err[k];
	//				res_err[k] = res_err[j];
	//				res_err[j] = tr;
	//			}
	//		}
	//	}
	//	if (noerr[i] > 0) {
	//		double at = log(1.0 - noerr[i] / para_module[i][0]);
	//		/*para_module[i][1] = 0.0 - at / ind->xreal[i];*/
	//		para_module[i][1] = 0.0 - at / res_err[noerr[i] - 1];
	//	}
	//	para_module[i][0] = para_module[i][0] - noerr[i];
	//	/*delete[]res_err;*/

	//}
	//getchar();

};

void re_parameter1(individual *ind, double &cost, int no)//子系统为6
{
	//CLEFTTOP::OnBnClickedButalog();
	int noerr[maxvar2];
	int i = 0;
	for (int k = 0; k< nsystem; k++) {
		for (int j = 0; j < no_subsys1[k]; j++) {
			noerr[i] = int(para_module1[i][0] * err_detect1[no][i]);
			/*noerr[i] = rnd(0, int(para_module[i][0]));*/
			double res_err[350];
			int on = 0;
			while (on < noerr[i]) {
				if (ind->xreal[i] < 0.1) { noerr[i] = 0; break; }
				/*double s = randomperc()* ind->xreal[i];*/
				double s = randomperc()*ind->xreal[i]; /*ind->fitness[1] /nsystem/ no_subsys[k];*/
				double pr = 1 - exp(-para_module1[i][1] * s);
				double rand = randomperc();
				if (rand < pr) {
					res_err[on] = s;
					on++;

				}
			}
			for (int p = 0; p < noerr[i]; p++) {
				for (int q = p + 1; q < noerr[i]; q++) {
					if (res_err[p] > res_err[q]) {
						double tr = res_err[q];
						res_err[q] = res_err[p];
						res_err[p] = tr;
					}
				}
			}
			if (noerr[i] > 0) {
				double at = log(1.0 - noerr[i] / para_module1[i][0]);
				/*para_module[i][1] = 0.0 - at / ind->xreal[i];*/
				para_module1[i][1] = 0.0 - at / res_err[noerr[i] - 1];
			}
			para_module1[i][0] = para_module1[i][0] - noerr[i];
			i++;
		}
	}
	//for (int i = 0; i < nvar; i++) {
	//	/*noerr[i] = rnd(0,int(para_module[i][0]));*/
	//	noerr[i] =  int(para_module1[i][0]*tperiod[no-1]/(T_MAX_RESOURCE-tperiod[noperiods-1]));
	//	/*double *res_err = new double[noerr[i]];*/
	//	double res_err[350];
	//	int on = 0;
	//	while (on < noerr[i]) {
	//		/*if (ind->xreal[i] < 0.01) { noerr[i] = 0; break; }*/
	//		/*double s = randomperc()* ind->xreal[i];*/
	//		double s = randomperc()* tperiod[no-1]/nvar;
	//		double pr = 1 - exp(-para_module[i][1] * s);
	//		double rand = randomperc();
	//		if (rand < pr) {
	//			res_err[on] = s;
	//			on++;

	//		}
	//	}
	//	for (int j = 0; j < noerr[i]; j++) {
	//		for (int k = j + 1; k < noerr[i]; k++) {
	//			if (res_err[j] > res_err[k]) {
	//				double tr = res_err[k];
	//				res_err[k] = res_err[j];
	//				res_err[j] = tr;
	//			}
	//		}
	//	}
	//	if (noerr[i] > 0) {
	//		double at = log(1.0 - noerr[i] / para_module[i][0]);
	//		/*para_module[i][1] = 0.0 - at / ind->xreal[i];*/
	//		para_module[i][1] = 0.0 - at / res_err[noerr[i] - 1];
	//	}
	//	para_module[i][0] = para_module[i][0] - noerr[i];
	//	/*delete[]res_err;*/

	//}
	//getchar();

};
void re_parameter2(individual *ind, double &cost, int no)//子系统为7
{
	//CLEFTTOP::OnBnClickedButalog();
	int noerr[maxvar2];
	int i = 0;
	for (int k = 0; k< nsystem; k++) {
		for (int j = 0; j < no_subsys2[k]; j++) {
			noerr[i] = int(para_module2[i][0] * err_detect2[no][i]);
			/*noerr[i] = rnd(0, int(para_module[i][0]));*/
			double res_err[350];
			int on = 0;
			while (on < noerr[i]) {
				if (ind->xreal[i] < 0.1) { noerr[i] = 0; break; }
				/*double s = randomperc()* ind->xreal[i];*/
				double s = randomperc()*ind->xreal[i]; /*ind->fitness[1] /nsystem/ no_subsys[k];*/
				double pr = 1 - exp(-para_module2[i][1] * s);
				double rand = randomperc();
				if (rand < pr) {
					res_err[on] = s;
					on++;

				}
			}
			for (int p = 0; p < noerr[i]; p++) {
				for (int q = p + 1; q < noerr[i]; q++) {
					if (res_err[p] > res_err[q]) {
						double tr = res_err[q];
						res_err[q] = res_err[p];
						res_err[p] = tr;
					}
				}
			}
			if (noerr[i] > 0) {
				double at = log(1.0 - noerr[i] / para_module2[i][0]);
				/*para_module[i][1] = 0.0 - at / ind->xreal[i];*/
				para_module2[i][1] = 0.0 - at / res_err[noerr[i] - 1];
			}
			para_module2[i][0] = para_module2[i][0] - noerr[i];
			i++;
		}
	}
	//for (int i = 0; i < nvar; i++) {
	//	/*noerr[i] = rnd(0,int(para_module[i][0]));*/
	//	noerr[i] =  int(para_module1[i][0]*tperiod[no-1]/(T_MAX_RESOURCE-tperiod[noperiods-1]));
	//	/*double *res_err = new double[noerr[i]];*/
	//	double res_err[350];
	//	int on = 0;
	//	while (on < noerr[i]) {
	//		/*if (ind->xreal[i] < 0.01) { noerr[i] = 0; break; }*/
	//		/*double s = randomperc()* ind->xreal[i];*/
	//		double s = randomperc()* tperiod[no-1]/nvar;
	//		double pr = 1 - exp(-para_module[i][1] * s);
	//		double rand = randomperc();
	//		if (rand < pr) {
	//			res_err[on] = s;
	//			on++;

	//		}
	//	}
	//	for (int j = 0; j < noerr[i]; j++) {
	//		for (int k = j + 1; k < noerr[i]; k++) {
	//			if (res_err[j] > res_err[k]) {
	//				double tr = res_err[k];
	//				res_err[k] = res_err[j];
	//				res_err[j] = tr;
	//			}
	//		}
	//	}
	//	if (noerr[i] > 0) {
	//		double at = log(1.0 - noerr[i] / para_module[i][0]);
	//		/*para_module[i][1] = 0.0 - at / ind->xreal[i];*/
	//		para_module[i][1] = 0.0 - at / res_err[noerr[i] - 1];
	//	}
	//	para_module[i][0] = para_module[i][0] - noerr[i];
	//	/*delete[]res_err;*/

	//}
	//getchar();

};
void nselect(population *old_pop_ptr, population *pop2_ptr)
{
	int *fit_ptr1, *fit_ptr2;

	double rnd2, *f1_ptr, *f2_ptr;

	double *select_ptr_r, *s1_ptr_r, *s2_ptr_r;

	individual *j, *j1;

	int i, rnd, rnd1, k, n, j2, r;

	old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);

	pop2_ptr->ind_ptr = &(pop2_ptr->ind[0]);

	j = &(old_pop_ptr->ind[popsize - 1]);

	old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);
	j2 = 0;
	r = popsize;

	for (n = 0, k = 0; n < popsize; n++, k++)
	{
		pop2_ptr->ind_ptr = &(pop2_ptr->ind[k]);
		select_ptr_r = &(pop2_ptr->ind_ptr->xreal[0]);

		rnd2 = randomperc();

		rnd2 = popsize* rnd2;

		rnd = (int)floor(rnd2);

		if (rnd == 0)
			rnd = popsize - k;

		if (rnd == popsize)
			rnd = (popsize - 2) / 2;

		/*Select first parent randomly*/
		j = &(old_pop_ptr->ind[rnd - 1]);

		rnd2 = randomperc();

		rnd2 = popsize * rnd2;

		rnd1 = (int)floor(rnd2);

		if (rnd1 == 0)
			rnd1 = popsize - n;

		if (rnd1 == popsize)
			rnd1 = (popsize - 4) / 2;

		/*Select second parent randomly*/
		j1 = &(old_pop_ptr->ind[rnd1 - 1]);

		old_pop_ptr->ind_ptr = j;

		s1_ptr_r = &(old_pop_ptr->ind_ptr->xreal[0]);
		fit_ptr1 = &(old_pop_ptr->ind_ptr->rank);
		f1_ptr = &(old_pop_ptr->ind_ptr->cub_len);

		old_pop_ptr->ind_ptr = j1;
		s2_ptr_r = &(old_pop_ptr->ind_ptr->xreal[0]);
		fit_ptr2 = &(old_pop_ptr->ind_ptr->rank);
		f2_ptr = &(old_pop_ptr->ind_ptr->cub_len);
		/*--------------------------------------------------------------------------*/

		/*------------------SELECTION PROCEDURE------------------------------------*/

		/*Comparing the fitnesses*/

		if (*fit_ptr1 > *fit_ptr2)
		{
			for (i = 0; i < nvar; i++)
				*select_ptr_r++ = *s2_ptr_r++;
		}
		else
		{
			if (*fit_ptr1 < *fit_ptr2)
			{
				for (i = 0; i < nvar; i++)
					*select_ptr_r++ = *s1_ptr_r++;
			}
			else
			{
				if (*f1_ptr < *f2_ptr)
				{
					for (i = 0; i < nvar; i++)
						*select_ptr_r++ = *s2_ptr_r++;
				}
				else
				{
					for (i = 0; i < nvar; i++)
						*select_ptr_r++ = *s1_ptr_r++;
				}
			}
		}
	}

};

void ged3()
{
	
	/*number of the variables*/
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun;
	if (popn == 0)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (popn == 1)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	if (gennumber == 0)
		gener = MAX_GENERATION;
	else
		if (gennumber == 1)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;
	nsystem = MAX_SYSTEM;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\complex\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	

}
void CLEFTTOP:: zq100sc0(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged100"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));

	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun;
	if (zqs == 50)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//if (gennumber == 0)
	gener = MAX_GENERATION1;
	//else
	//	if (gennumber == 1)
	//		gener = MAX_GENERATION1;
	//else
	//	gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\complex\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged100(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir100"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	if (zqs == 50)
		popsize = maxpop;
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/

	gener = MAX_GENERATION1;


	int int_exp = 0;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\complex\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE;
		//input();
		/*number of the variables*/

		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir100(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
void CLEFTTOP::zq250sc0(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged250"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));

	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun;
	if (zqs == 50)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//if (gennumber == 0)
	gener = MAX_GENERATION2;
	//else
	//	if (gennumber == 1)
	//		gener = MAX_GENERATION1;
	//else
	//	gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\complex\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged250(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir250"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	if (zqs == 50)
		popsize = maxpop;
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/

	gener = MAX_GENERATION2;


	int int_exp = 0;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\complex\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE;
		//input();
		/*number of the variables*/

		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir250(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
void CLEFTTOP::zq50sc1()
{
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged50"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar1;

	/*number of the functions*/
	nfunc = maxfun;
	if (zqs == 50)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//if (gennumber == 0)
	gener = MAX_GENERATION;

	nsystem = MAX_SYSTEM1;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod1[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter1(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod1[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func1(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged50(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
		CString msql1;
		//msq2.Format(_T())
		msql1.Format(_T("drop table if exists gedir50"));
		char *sql11 = CSt2char(msql1);
		if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("清空表成功！"));
		}
		int int_exp = 0;
		nvar = maxvar1;

		/*number of the functions*/
		nfunc = maxfun1;

		/*number of the individuals in the population*/
		if (zqs == 50)
			popsize = maxpop;
		else
			if (zqs == 100)
				popsize = maxpop1;
			else
				popsize = maxpop2;

		/*No. of generations*/
		//if (gennumber == 0)
		gener = MAX_GENERATION;

		for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
		{
			printf_s("Run experiment %d!\n", int_exp + 1);

			//	sleep((clock_t)3 * CLOCKS_PER_SEC);

			FILE *end_ptr;
			char file1[500];

			sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
			fopen_s(&end_ptr, file1, "w+");


			//clock_t start, finish;
			//	double duration;

			int i, j, l, f;

			population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

			time_stage = T_MAX_RESOURCE1;
			//input();
			/*number of the variables*/


			/*the crossover probability (between 0.5 and 1)*/
			pcross = 0.9;

			/*the mutation probability*/
			pmut_r = 0.1;

			/*Distribution Index for real-coded crossover between 0.5 to 100*/
			di = 100;

			/*Distribution Index for real-coded mutation between 0.5 to 500*/
			dim = 500;

			seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinitir(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			funcir1(old_pop_ptr);/*Function Calculation*/

			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				real_mutate(new_pop_ptr); /*Real Mutation*/

				new_pop_ptr = &(newpop);
				funcir1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/

				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}

					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/
				if (i == gener - 1)
				{  // for the last generation 
					old_pop_ptr = &(matepop);
					for (f = 0; f < popsize; f++) // for printing
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

						// for all feasible solutions and non-dominated solutions
						if (old_pop_ptr->ind_ptr->rank == 1)
						{
							for (l = 0; l < nfunc; l++)
							{
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}
							fprintf(end_ptr, "%s", "\n");


						}  // feasibility check
					} // end of f (printing)
				}

			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);

			fclose(end_ptr);
		}

		//fclose(run_time);

		//	printf_s("please input a char to exit the program!\n");

		//	getchar();
		MessageBoxW(_T("运行结束"));
		MessageBoxW(_T("将文件内容写入数据库"));
		CString mstrsql5;
		mstrsql5.Format(_T("CREATE TABLE gedir50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
		char *sql1 = CSt2char(mstrsql5);
		if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("建表成功！"));
		}
		for (int fwj = 0; fwj < 30; fwj++)
		{
			char  mstrsql1[300], mstrsql2[200];
			sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
			CString msq(mstrsql2), msqll1(mstrsql1);
			//sprintf_s(mstrsql1,);
			msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir50(pop,ben,gen)"));
			char *sql2 = CSt2char(msqll1);
			if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
			{
				MessageBoxW(_T("填表失败"));
			}
			else
			{
				if (fwj == 29)
					MessageBoxW(_T("数据库存储数据成功"));
				continue;
			}
		}
	}
}
void CLEFTTOP::zq100sc1(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged100"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar1;

	/*number of the functions*/
	nfunc = maxfun;
	if (zqs == 50)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//if (gennumber == 0)
	gener = MAX_GENERATION1;

	nsystem = MAX_SYSTEM1;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod1[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter1(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod1[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func1(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged100(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
		CString msql1;
		//msq2.Format(_T())
		msql1.Format(_T("drop table if exists gedir100"));
		char *sql11 = CSt2char(msql1);
		if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("清空表成功！"));
		}
		int int_exp = 0;
		nvar = maxvar1;

		/*number of the functions*/
		nfunc = maxfun1;

		/*number of the individuals in the population*/
		if (zqs == 50)
			popsize = maxpop;
		else
			if (zqs == 100)
				popsize = maxpop1;
			else
				popsize = maxpop2;

		/*No. of generations*/
		//if (gennumber == 0)
		gener = MAX_GENERATION1;

		for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
		{
			printf_s("Run experiment %d!\n", int_exp + 1);

			//	sleep((clock_t)3 * CLOCKS_PER_SEC);

			FILE *end_ptr;
			char file1[500];

			sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
			fopen_s(&end_ptr, file1, "w+");


			//clock_t start, finish;
			//	double duration;

			int i, j, l, f;

			population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

			time_stage = T_MAX_RESOURCE1;
			//input();
			/*number of the variables*/


			/*the crossover probability (between 0.5 and 1)*/
			pcross = 0.9;

			/*the mutation probability*/
			pmut_r = 0.1;

			/*Distribution Index for real-coded crossover between 0.5 to 100*/
			di = 100;

			/*Distribution Index for real-coded mutation between 0.5 to 500*/
			dim = 500;

			seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinitir(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			funcir1(old_pop_ptr);/*Function Calculation*/

			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				real_mutate(new_pop_ptr); /*Real Mutation*/

				new_pop_ptr = &(newpop);
				funcir1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/

				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}

					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/
				if (i == gener - 1)
				{  // for the last generation 
					old_pop_ptr = &(matepop);
					for (f = 0; f < popsize; f++) // for printing
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

						// for all feasible solutions and non-dominated solutions
						if (old_pop_ptr->ind_ptr->rank == 1)
						{
							for (l = 0; l < nfunc; l++)
							{
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}
							fprintf(end_ptr, "%s", "\n");


						}  // feasibility check
					} // end of f (printing)
				}

			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);

			fclose(end_ptr);
		}

		//fclose(run_time);

		//	printf_s("please input a char to exit the program!\n");

		//	getchar();
		MessageBoxW(_T("运行结束"));
		MessageBoxW(_T("将文件内容写入数据库"));
		CString mstrsql5;
		mstrsql5.Format(_T("CREATE TABLE gedir100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
		char *sql1 = CSt2char(mstrsql5);
		if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("建表成功！"));
		}
		for (int fwj = 0; fwj < 30; fwj++)
		{
			char  mstrsql1[300], mstrsql2[200];
			sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
			CString msq(mstrsql2), msqll1(mstrsql1);
			//sprintf_s(mstrsql1,);
			msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir100(pop,ben,gen)"));
			char *sql2 = CSt2char(msqll1);
			if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
			{
				MessageBoxW(_T("填表失败"));
			}
			else
			{
				if (fwj == 29)
					MessageBoxW(_T("数据库存储数据成功"));
				continue;
			}
		}
	}
}
void CLEFTTOP::zq250sc1(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged250"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2 ,strlen(sql2)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar1;

	/*number of the functions*/
	nfunc = maxfun;
	if (zqs == 50)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//if (gennumber == 0)
	gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM1;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod1[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter1(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod1[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func1(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged250(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
		CString msql1;
		//msq2.Format(_T())
		msql1.Format(_T("drop table if exists gedir250"));
		char *sql11 = CSt2char(msql1);
		if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("清空表成功！"));
		}
		int int_exp = 0;
		nvar = maxvar1;

		/*number of the functions*/
		nfunc = maxfun1;

		/*number of the individuals in the population*/
		if (zqs == 50)
			popsize = maxpop;
		else
			if (zqs == 100)
				popsize = maxpop1;
			else
				popsize = maxpop2;

		/*No. of generations*/
		//if (gennumber == 0)
		gener = MAX_GENERATION2;

		for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
		{
			printf_s("Run experiment %d!\n", int_exp + 1);

			//	sleep((clock_t)3 * CLOCKS_PER_SEC);

			FILE *end_ptr;
			char file1[500];

			sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
			fopen_s(&end_ptr, file1, "w+");


			//clock_t start, finish;
			//	double duration;

			int i, j, l, f;

			population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

			time_stage = T_MAX_RESOURCE1;
			//input();
			/*number of the variables*/


			/*the crossover probability (between 0.5 and 1)*/
			pcross = 0.9;

			/*the mutation probability*/
			pmut_r = 0.1;

			/*Distribution Index for real-coded crossover between 0.5 to 100*/
			di = 100;

			/*Distribution Index for real-coded mutation between 0.5 to 500*/
			dim = 500;

			seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinitir(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			funcir1(old_pop_ptr);/*Function Calculation*/

			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				real_mutate(new_pop_ptr); /*Real Mutation*/

				new_pop_ptr = &(newpop);
				funcir1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/

				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}

					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/
				if (i == gener - 1)
				{  // for the last generation 
					old_pop_ptr = &(matepop);
					for (f = 0; f < popsize; f++) // for printing
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

						// for all feasible solutions and non-dominated solutions
						if (old_pop_ptr->ind_ptr->rank == 1)
						{
							for (l = 0; l < nfunc; l++)
							{
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}
							fprintf(end_ptr, "%s", "\n");


						}  // feasibility check
					} // end of f (printing)
				}

			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);

			fclose(end_ptr);
		}

		//fclose(run_time);

		//	printf_s("please input a char to exit the program!\n");

		//	getchar();
		MessageBoxW(_T("运行结束"));
		MessageBoxW(_T("将文件内容写入数据库"));
		CString mstrsql5;
		mstrsql5.Format(_T("CREATE TABLE gedir250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
		char *sql1 = CSt2char(mstrsql5);
		if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("建表成功！"));
		}
		for (int fwj = 0; fwj < 30; fwj++)
		{
			char  mstrsql1[300], mstrsql2[200];
			sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
			CString msq(mstrsql2), msqll1(mstrsql1);
			//sprintf_s(mstrsql1,);
			msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir250(pop,ben,gen)"));
			char *sql2 = CSt2char(msqll1);
			if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
			{
				MessageBoxW(_T("填表失败"));
			}
			else
			{
				if (fwj == 29)
					MessageBoxW(_T("数据库存储数据成功"));
				continue;
			}
		}
	}
}
void CLEFTTOP::zq50sc2(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged50"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		CWnd::MessageBox(_T("失败"));
	}
	else
	{
		MessageBox(_T("清空表成功！"));
	}
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun;
	if (zqs == 50)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
//	if (gennumber == 0)
		gener = MAX_GENERATION;
	nsystem = MAX_SYSTEM2;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger1\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod2[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter2(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod2[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func2(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged50(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir50"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	int int_exp = 0;
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	if (zqs == 50)
		popsize = maxpop;
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//if (gennumber == 0)
		gener = MAX_GENERATION;
	
	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger1\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE2;
		//input();
		/*number of the variables*/


		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir2(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir50(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
			
}
void CLEFTTOP::zq100sc2(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged100"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		CWnd::MessageBox(_T("失败"));
	}
	else
	{
		MessageBox(_T("清空表成功！"));
	}
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun;
	if (zqs == 50)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//	if (gennumber == 0)
	gener = MAX_GENERATION1;
	nsystem = MAX_SYSTEM2;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger1\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod2[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter2(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod2[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func2(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged100(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir100"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	int int_exp = 0;
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	if (zqs == 50)
		popsize = maxpop;
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//if (gennumber == 0)
	gener = MAX_GENERATION1;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger1\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE2;
		//input();
		/*number of the variables*/


		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir2(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir100(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
void CLEFTTOP::zq250sc2(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged250"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		CWnd::MessageBox(_T("失败"));
	}
	else
	{
		MessageBox(_T("清空表成功！"));
	}
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun;
	if (zqs == 50)
	{/*number of the individuals in the population*/
		popsize = maxpop;
	}
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//	if (gennumber == 0)
	gener = MAX_GENERATION2;
	nsystem = MAX_SYSTEM2;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger1\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod2[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter2(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod2[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func2(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged250(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir250"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	int int_exp = 0;
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	if (zqs == 50)
		popsize = maxpop;
	else
		if (zqs == 100)
			popsize = maxpop1;
		else
			popsize = maxpop2;

	/*No. of generations*/
	//if (gennumber == 0)
	gener = MAX_GENERATION2;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger1\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE2;
		//input();
		/*number of the variables*/


		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir2(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir250(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
/****************************a迭代模式下*****************/
void CLEFTTOP::gn50sc0(){
		CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged50"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));

	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;
	zqs = 50;
	/*number of the functions*/
	nfunc = maxfun;
	
	/*number of the individuals in the population*/
		popsize = maxpop;
		popn = 0;

	/*No. of generations*/
	//if (gennumber == 0)
		if (das==50)
	gener = MAX_GENERATION;
		else
			if (das==100)
				gener = MAX_GENERATION1;
			else
				gener = MAX_GENERATION2;
	//else
	//	if (gennumber == 1)
	//		gener = MAX_GENERATION1;
	//else
	//	gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\complex\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged50(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir50"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	popsize = maxpop;


	/*No. of generations*/
	//if (gennumber == 0)
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;


	int int_exp = 0;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\complex\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE;
		//input();
		/*number of the variables*/

		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir50(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
void CLEFTTOP::gn100sc0(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged100"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));

	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun;
	popsize = maxpop1;
	popn = 1;
	zqs = 100;
	/*No. of generations*/
	//if (gennumber == 0)
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;
	//else
	//	if (gennumber == 1)
	//		gener = MAX_GENERATION1;
	//else
	//	gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\complex\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged100(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir100"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	popsize = maxpop1;


	/*No. of generations*/
	//if (gennumber == 0)
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;


	int int_exp = 0;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\complex\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE;
		//input();
		/*number of the variables*/

		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir100(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
void CLEFTTOP::gn250sc0(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged250"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));

	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;
	zqs = 250;
	/*number of the functions*/
	nfunc = maxfun;
	
		popsize = maxpop2;
		popn = 2;

	/*No. of generations*/
	if (das == 50)
	gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
	else
		gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\complex\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged250(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir250"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	//if (zqs == 50)
	//	popsize = maxpop;
	//else
	//	if (zqs == 100)
	//		popsize = maxpop1;
		//else
		//	popsize = maxpop2;

	/*No. of generations*/

	//gener = MAX_GENERATION2;


	int int_exp = 0;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\complex\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE;
		//input();
		/*number of the variables*/

		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir250(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
void CLEFTTOP::gn50sc1()
{
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged50"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar1;

	/*number of the functions*/
	nfunc = maxfun;
	
		popsize = maxpop;
		popn = 0;


	/*No. of generations*/
	if (das == 50)
	gener = MAX_GENERATION;
	else
		if (das==100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM1;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod1[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter1(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod1[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func1(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged50(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
		CString msql1;
		//msq2.Format(_T())
		msql1.Format(_T("drop table if exists gedir50"));
		char *sql11 = CSt2char(msql1);
		if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("清空表成功！"));
		}
		int int_exp = 0;
		nvar = maxvar1;

		/*number of the functions*/
		nfunc = maxfun1;

		/*number of the individuals in the population*/
		popsize = maxpop;



		/*No. of generations*/
		if (das == 50)
			gener = MAX_GENERATION;
		else
			if (das == 100)
				gener = MAX_GENERATION1;
			else
				gener = MAX_GENERATION2;

		for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
		{
			printf_s("Run experiment %d!\n", int_exp + 1);

			//	sleep((clock_t)3 * CLOCKS_PER_SEC);

			FILE *end_ptr;
			char file1[500];

			sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
			fopen_s(&end_ptr, file1, "w+");


			//clock_t start, finish;
			//	double duration;

			int i, j, l, f;

			population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

			time_stage = T_MAX_RESOURCE1;
			//input();
			/*number of the variables*/


			/*the crossover probability (between 0.5 and 1)*/
			pcross = 0.9;

			/*the mutation probability*/
			pmut_r = 0.1;

			/*Distribution Index for real-coded crossover between 0.5 to 100*/
			di = 100;

			/*Distribution Index for real-coded mutation between 0.5 to 500*/
			dim = 500;

			seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinitir(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			funcir1(old_pop_ptr);/*Function Calculation*/

			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				real_mutate(new_pop_ptr); /*Real Mutation*/

				new_pop_ptr = &(newpop);
				funcir1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/

				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}

					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/
				if (i == gener - 1)
				{  // for the last generation 
					old_pop_ptr = &(matepop);
					for (f = 0; f < popsize; f++) // for printing
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

						// for all feasible solutions and non-dominated solutions
						if (old_pop_ptr->ind_ptr->rank == 1)
						{
							for (l = 0; l < nfunc; l++)
							{
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}
							fprintf(end_ptr, "%s", "\n");


						}  // feasibility check
					} // end of f (printing)
				}

			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);

			fclose(end_ptr);
		}

		//fclose(run_time);

		//	printf_s("please input a char to exit the program!\n");

		//	getchar();
		MessageBoxW(_T("运行结束"));
		MessageBoxW(_T("将文件内容写入数据库"));
		CString mstrsql5;
		mstrsql5.Format(_T("CREATE TABLE gedir50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
		char *sql1 = CSt2char(mstrsql5);
		if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("建表成功！"));
		}
		for (int fwj = 0; fwj < 30; fwj++)
		{
			char  mstrsql1[300], mstrsql2[200];
			sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
			CString msq(mstrsql2), msqll1(mstrsql1);
			//sprintf_s(mstrsql1,);
			msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir50(pop,ben,gen)"));
			char *sql2 = CSt2char(msqll1);
			if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
			{
				MessageBoxW(_T("填表失败"));
			}
			else
			{
				if (fwj == 29)
					MessageBoxW(_T("数据库存储数据成功"));
				continue;
			}
		}
	}
}
void CLEFTTOP::gn100sc1(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged100"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar1;

	/*number of the functions*/
	nfunc = maxfun;
	popsize = maxpop1;
	popn = 1;


	/*No. of generations*/
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM1;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod1[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter1(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod1[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func1(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged100(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
		CString msql1;
		//msq2.Format(_T())
		msql1.Format(_T("drop table if exists gedir100"));
		char *sql11 = CSt2char(msql1);
		if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("清空表成功！"));
		}
		int int_exp = 0;
		nvar = maxvar1;

		/*number of the functions*/
		nfunc = maxfun1;

		/*number of the individuals in the population*/
		popsize = maxpop1;



		/*No. of generations*/
		if (das == 50)
			gener = MAX_GENERATION;
		else
			if (das == 100)
				gener = MAX_GENERATION1;
			else
				gener = MAX_GENERATION2;

		for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
		{
			printf_s("Run experiment %d!\n", int_exp + 1);

			//	sleep((clock_t)3 * CLOCKS_PER_SEC);

			FILE *end_ptr;
			char file1[500];

			sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
			fopen_s(&end_ptr, file1, "w+");


			//clock_t start, finish;
			//	double duration;

			int i, j, l, f;

			population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

			time_stage = T_MAX_RESOURCE1;
			//input();
			/*number of the variables*/


			/*the crossover probability (between 0.5 and 1)*/
			pcross = 0.9;

			/*the mutation probability*/
			pmut_r = 0.1;

			/*Distribution Index for real-coded crossover between 0.5 to 100*/
			di = 100;

			/*Distribution Index for real-coded mutation between 0.5 to 500*/
			dim = 500;

			seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinitir(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			funcir1(old_pop_ptr);/*Function Calculation*/

			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				real_mutate(new_pop_ptr); /*Real Mutation*/

				new_pop_ptr = &(newpop);
				funcir1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/

				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}

					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/
				if (i == gener - 1)
				{  // for the last generation 
					old_pop_ptr = &(matepop);
					for (f = 0; f < popsize; f++) // for printing
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

						// for all feasible solutions and non-dominated solutions
						if (old_pop_ptr->ind_ptr->rank == 1)
						{
							for (l = 0; l < nfunc; l++)
							{
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}
							fprintf(end_ptr, "%s", "\n");


						}  // feasibility check
					} // end of f (printing)
				}

			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);

			fclose(end_ptr);
		}

		//fclose(run_time);

		//	printf_s("please input a char to exit the program!\n");

		//	getchar();
		MessageBoxW(_T("运行结束"));
		MessageBoxW(_T("将文件内容写入数据库"));
		CString mstrsql5;
		mstrsql5.Format(_T("CREATE TABLE gedir100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
		char *sql1 = CSt2char(mstrsql5);
		if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("建表成功！"));
		}
		for (int fwj = 0; fwj < 30; fwj++)
		{
			char  mstrsql1[300], mstrsql2[200];
			sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
			CString msq(mstrsql2), msqll1(mstrsql1);
			//sprintf_s(mstrsql1,);
			msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir100(pop,ben,gen)"));
			char *sql2 = CSt2char(msqll1);
			if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
			{
				MessageBoxW(_T("填表失败"));
			}
			else
			{
				if (fwj == 29)
					MessageBoxW(_T("数据库存储数据成功"));
				continue;
			}
		}
	}
}
void CLEFTTOP::gn250sc1(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged250"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	nvar = maxvar1;

	/*number of the functions*/
	nfunc = maxfun;
	popsize = maxpop2;
	popn = 2;


	/*No. of generations*/
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;

	nsystem = MAX_SYSTEM1;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod1[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter1(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod1[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func1(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func1(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged250(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
		CString msql1;
		//msq2.Format(_T())
		msql1.Format(_T("drop table if exists gedir250"));
		char *sql11 = CSt2char(msql1);
		if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("清空表成功！"));
		}
		int int_exp = 0;
		nvar = maxvar1;

		/*number of the functions*/
		nfunc = maxfun1;

		popsize = maxpop2;



		/*No. of generations*/
		if (das == 50)
			gener = MAX_GENERATION;
		else
			if (das == 100)
				gener = MAX_GENERATION1;
			else
				gener = MAX_GENERATION2;

		for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
		{
			printf_s("Run experiment %d!\n", int_exp + 1);

			//	sleep((clock_t)3 * CLOCKS_PER_SEC);

			FILE *end_ptr;
			char file1[500];

			sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
			fopen_s(&end_ptr, file1, "w+");


			//clock_t start, finish;
			//	double duration;

			int i, j, l, f;

			population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

			time_stage = T_MAX_RESOURCE1;
			//input();
			/*number of the variables*/


			/*the crossover probability (between 0.5 and 1)*/
			pcross = 0.9;

			/*the mutation probability*/
			pmut_r = 0.1;

			/*Distribution Index for real-coded crossover between 0.5 to 100*/
			di = 100;

			/*Distribution Index for real-coded mutation between 0.5 to 500*/
			dim = 500;

			seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinitir(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			funcir1(old_pop_ptr);/*Function Calculation*/

			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				real_mutate(new_pop_ptr); /*Real Mutation*/

				new_pop_ptr = &(newpop);
				funcir1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/

				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}

					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/
				if (i == gener - 1)
				{  // for the last generation 
					old_pop_ptr = &(matepop);
					for (f = 0; f < popsize; f++) // for printing
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

						// for all feasible solutions and non-dominated solutions
						if (old_pop_ptr->ind_ptr->rank == 1)
						{
							for (l = 0; l < nfunc; l++)
							{
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}
							fprintf(end_ptr, "%s", "\n");


						}  // feasibility check
					} // end of f (printing)
				}

			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);

			fclose(end_ptr);
		}

		//fclose(run_time);

		//	printf_s("please input a char to exit the program!\n");

		//	getchar();
		MessageBoxW(_T("运行结束"));
		MessageBoxW(_T("将文件内容写入数据库"));
		CString mstrsql5;
		mstrsql5.Format(_T("CREATE TABLE gedir250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
		char *sql1 = CSt2char(mstrsql5);
		if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("建表成功！"));
		}
		for (int fwj = 0; fwj < 30; fwj++)
		{
			char  mstrsql1[300], mstrsql2[200];
			sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
			CString msq(mstrsql2), msqll1(mstrsql1);
			//sprintf_s(mstrsql1,);
			msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir250(pop,ben,gen)"));
			char *sql2 = CSt2char(msqll1);
			if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
			{
				MessageBoxW(_T("填表失败"));
			}
			else
			{
				if (fwj == 29)
					MessageBoxW(_T("数据库存储数据成功"));
				continue;
			}
		}
	}
}
void CLEFTTOP::gn50sc2(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged50"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		CWnd::MessageBox(_T("失败"));
	}
	else
	{
		MessageBox(_T("清空表成功！"));
	}
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun;
	popsize = maxpop;
	popn = 0;


	/*No. of generations*/
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;
	nsystem = MAX_SYSTEM2;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger1\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod2[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter2(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod2[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func2(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged50(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir50"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	int int_exp = 0;
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	popsize = maxpop;



	/*No. of generations*/
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger1\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE2;
		//input();
		/*number of the variables*/


		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir2(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir50(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}

}
void CLEFTTOP::gn100sc2(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged100"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		CWnd::MessageBox(_T("失败"));
	}
	else
	{
		MessageBox(_T("清空表成功！"));
	}
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun;
	popsize = maxpop1;
	popn = 1;


	/*No. of generations*/
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;
	nsystem = MAX_SYSTEM2;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger1\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod2[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter2(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod2[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func2(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged100(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir100"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	int int_exp = 0;
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	popsize = maxpop1;



	/*No. of generations*/
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger1\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE2;
		//input();
		/*number of the variables*/


		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir2(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir100(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir100(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
void CLEFTTOP::gn250sc2(){
	CString msql;
	//msq2.Format(_T())
	msql.Format(_T("drop table if exists ged250"));
	char *sql2 = CSt2char(msql);
	if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
	{
		CWnd::MessageBox(_T("失败"));
	}
	else
	{
		MessageBox(_T("清空表成功！"));
	}
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun;
	popsize = maxpop2;
	popn = 2;


	/*No. of generations*/
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;
	nsystem = MAX_SYSTEM2;

	/*Max no. of variables*/

	//#define maxvar    30


	//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
	//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
	//	};//large-scale


	//double para_module[maxvar][2];

	/********************************************************************/


	/*Max value of testing time*/

	//#define T_MAX_RESOURCE 150000.0



	/*Max no. of subsystems*/


	/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







	//void re_parameter(individual *ind, double &cost, int no);
	//void input(); /*Input Parameters from user*/

	//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
	//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
	//void ranking(population *pop_ptr);
	//int indcmp(double *ptr1, double *ptr2);
	//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
	//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
	//void grank(int gen, int y);
	//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

	//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
	//void gshare(int rnk);/*Sharing the fitness*/
	//void sort(int m1);

	//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

	/********************************************************************/

	//int int_exp;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger1\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;





		//input();

		for (int nop = 0; nop < noperiods; nop++) {
			old_pop_ptr = &(oldpop);
			sumre += tperiod2[nop];
			if (nop > 0) {
				individual *ind = &(old_pop_ptr->ind[0]);
				for (int i = 0; i < popsize; i++) {
					if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
						ind = &(old_pop_ptr->ind[i]);
					}
				}
				costre += ind->fitness[1];
				re_parameter2(ind, costre, nop - 1);

				time_stage = sumre - costre;
			}
			else {
				time_stage = tperiod2[nop];
			}


			seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
			srand((unsigned)time(NULL));
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinit(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			func2(old_pop_ptr);	/*Function Calculation*/
			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				func2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/
				mate_pop_ptr = &(matepop);
				func2(mate_pop_ptr);
				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				/*	func(new_pop_ptr);
				new_pop_ptr = &(matepop);*/
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}
					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/


			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);
			if (nop == noperiods - 1)
			{  // for the last period
				old_pop_ptr = &(oldpop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							if (l == 1) {
								fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

							}
							else {
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}

						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}


		}
		fclose(end_ptr);
	}


	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();

	//return 0;
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql;
	mstrsql.Format(_T("CREATE TABLE ged250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged250(pop,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
	CString msql1;
	//msq2.Format(_T())
	msql1.Format(_T("drop table if exists gedir250"));
	char *sql11 = CSt2char(msql1);
	if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("清空表成功！"));
	}
	int int_exp = 0;
	nvar = maxvar2;

	/*number of the functions*/
	nfunc = maxfun1;

	/*number of the individuals in the population*/
	popsize = maxpop2;



	/*No. of generations*/
	if (das == 50)
		gener = MAX_GENERATION;
	else
		if (das == 100)
			gener = MAX_GENERATION1;
		else
			gener = MAX_GENERATION2;

	for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
	{
		printf_s("Run experiment %d!\n", int_exp + 1);

		//	sleep((clock_t)3 * CLOCKS_PER_SEC);

		FILE *end_ptr;
		char file1[500];

		sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger1\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
		fopen_s(&end_ptr, file1, "w+");


		//clock_t start, finish;
		//	double duration;

		int i, j, l, f;

		population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

		time_stage = T_MAX_RESOURCE2;
		//input();
		/*number of the variables*/


		/*the crossover probability (between 0.5 and 1)*/
		pcross = 0.9;

		/*the mutation probability*/
		pmut_r = 0.1;

		/*Distribution Index for real-coded crossover between 0.5 to 100*/
		di = 100;

		/*Distribution Index for real-coded mutation between 0.5 to 500*/
		dim = 500;

		seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
		warmup_random(seed);

		//	start = clock();
		/***********************************************/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			/*Initializing the Rank array having different individuals at a particular  rank to zero*/
			old_pop_ptr->rankno[j] = 0;
			new_pop_ptr->rankno[j] = 0;
			mate_pop_ptr->rankno[j] = 0;

			old_pop_ptr->ind[j].cub_len = 0.0;
			new_pop_ptr->ind[j].cub_len = 0.0;
			mate_pop_ptr->ind[j].cub_len = 0.0;
		}
		/*******************************************************/

		old_pop_ptr = &(oldpop);
		realinitir(old_pop_ptr);

		old_pop_ptr = &(oldpop);
		funcir2(old_pop_ptr);/*Function Calculation*/

		/********************************************************************/
		/*----------------------GENERATION STARTS HERE----------------------*/
		for (i = 0; i < gener; i++)
		{
			old_pop_ptr = &(oldpop);
			mate_pop_ptr = &(matepop);
			nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

			/********************************************************************/

			new_pop_ptr = &(newpop);
			real_mutate(new_pop_ptr); /*Real Mutation*/

			new_pop_ptr = &(newpop);
			funcir2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

			/********************************************************************/

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

			/********************************************************************/

			/********************************************************************/

			/*=======Copying the new population to old population======*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(matepop);
			for (j = 0; j < popsize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
				if (nvar > 0)
				{
					/*For Real Coded GA copying of the chromosomes*/
					for (l = 0; l < nvar; l++)
						old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
				}

				/*Copying the fitness vector */
				for (l = 0; l < nfunc; l++)
					old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

				/*Copying the crowd degree*/
				old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
			}   // end of j

			/*Copying the array having the record of the individual	at different ranks */
			for (l = 0; l < popsize; l++)
			{
				old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
			}
			/*Copying the maxrank */
			old_pop_ptr->maxrank = new_pop_ptr->maxrank;

			/********************************************************************/

			/*Printing the fitness record for last generation in a file last*/
			if (i == gener - 1)
			{  // for the last generation 
				old_pop_ptr = &(matepop);
				for (f = 0; f < popsize; f++) // for printing
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

					// for all feasible solutions and non-dominated solutions
					if (old_pop_ptr->ind_ptr->rank == 1)
					{
						for (l = 0; l < nfunc; l++)
						{
							fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
						}
						fprintf(end_ptr, "%s", "\n");


					}  // feasibility check
				} // end of f (printing)
			}

		}// end of i

		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	fprintf_s(run_time, "%3.4f ", duration);

		fclose(end_ptr);
	}

	//fclose(run_time);

	//	printf_s("please input a char to exit the program!\n");

	//	getchar();
	MessageBoxW(_T("运行结束"));
	MessageBoxW(_T("将文件内容写入数据库"));
	CString mstrsql1;
	mstrsql1.Format(_T("CREATE TABLE gedir250(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
	char *sql1 = CSt2char(mstrsql1);
	if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
	{
		MessageBoxW(_T("失败"));
	}
	else
	{
		MessageBoxW(_T("建表成功！"));
	}
	for (int fwj = 0; fwj < 30; fwj++)
	{
		char  mstrsql1[300], mstrsql2[200];
		sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
		CString msq(mstrsql2), msqll1(mstrsql1);
		//sprintf_s(mstrsql1,);
		msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir250(pop,ben,gen)"));
		char *sql2 = CSt2char(msqll1);
		if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
		{
			MessageBoxW(_T("填表失败"));
		}
		else
		{
			if (fwj == 29)
				MessageBoxW(_T("数据库存储数据成功"));
			continue;
		}
	}
}
void CLEFTTOP::datarun(){
	
	if (pn == 1)
	{

		if (zqs == 50)
			popn = 0;
		else
			if (zqs == 100)
				popn = 1;
			else
				popn = 2;
		if (zs == 0)
		{

			{
				CString msql;
				//msq2.Format(_T())
				msql.Format(_T("drop table if exists ged50"));
				char *sql2 = CSt2char(msql);
				if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
				{
					MessageBoxW(_T("失败"));

				}
				else
				{
					MessageBoxW(_T("清空表成功！"));
				}

				/*number of the variables*/
				nvar = maxvar;

				/*number of the functions*/
				nfunc = maxfun;
				if (zqs == 50)
				{/*number of the individuals in the population*/
					popsize = maxpop;
				}
				else
					if (zqs == 100)
						popsize = maxpop1;
					else
						popsize = maxpop2;

				/*No. of generations*/
				//if (gennumber == 0)
					gener = MAX_GENERATION;
				//else
				//	if (gennumber == 1)
				//		gener = MAX_GENERATION1;
				//	else
				//		gener = MAX_GENERATION2;
				nsystem = MAX_SYSTEM;

				/*Max no. of variables*/

				//#define maxvar    30


				//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
				//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
				//	};//large-scale


				//double para_module[maxvar][2];

				/********************************************************************/


				/*Max value of testing time*/

				//#define T_MAX_RESOURCE 150000.0



				/*Max no. of subsystems*/


				/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







				//void re_parameter(individual *ind, double &cost, int no);
				//void input(); /*Input Parameters from user*/

				//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
				//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
				//void ranking(population *pop_ptr);
				//int indcmp(double *ptr1, double *ptr2);
				//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
				//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
				//void grank(int gen, int y);
				//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

				//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
				//void gshare(int rnk);/*Sharing the fitness*/
				//void sort(int m1);

				//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

				/********************************************************************/

				//int int_exp;

				for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
				{
					printf_s("Run experiment %d!\n", int_exp + 1);

					//	sleep((clock_t)3 * CLOCKS_PER_SEC);

					FILE *end_ptr;
					char file1[500];

					sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\complex\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
					fopen_s(&end_ptr, file1, "w+");


					//clock_t start, finish;
					//	double duration;





					//input();

					for (int nop = 0; nop < noperiods; nop++) {
						old_pop_ptr = &(oldpop);
						sumre += tperiod[nop];
						if (nop > 0) {
							individual *ind = &(old_pop_ptr->ind[0]);
							for (int i = 0; i < popsize; i++) {
								if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
									ind = &(old_pop_ptr->ind[i]);
								}
							}
							costre += ind->fitness[1];
							re_parameter(ind, costre, nop - 1);

							time_stage = sumre - costre;
						}
						else {
							time_stage = tperiod[nop];
						}


						seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
						srand((unsigned)time(NULL));
						warmup_random(seed);

						//	start = clock();
						/***********************************************/
						old_pop_ptr = &(oldpop);
						new_pop_ptr = &(newpop);
						mate_pop_ptr = &(matepop);

						for (j = 0; j < popsize; j++)
						{
							/*Initializing the Rank array having different individuals at a particular  rank to zero*/
							old_pop_ptr->rankno[j] = 0;
							new_pop_ptr->rankno[j] = 0;
							mate_pop_ptr->rankno[j] = 0;

							old_pop_ptr->ind[j].cub_len = 0.0;
							new_pop_ptr->ind[j].cub_len = 0.0;
							mate_pop_ptr->ind[j].cub_len = 0.0;
						}
						/*******************************************************/

						old_pop_ptr = &(oldpop);
						realinit(old_pop_ptr);

						old_pop_ptr = &(oldpop);
						func(old_pop_ptr);	/*Function Calculation*/
						/********************************************************************/
						/*----------------------GENERATION STARTS HERE----------------------*/
						for (i = 0; i < gener; i++)
						{
							old_pop_ptr = &(oldpop);
							mate_pop_ptr = &(matepop);
							nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

							/********************************************************************/
							mate_pop_ptr = &(matepop);
							func(mate_pop_ptr);
							new_pop_ptr = &(newpop);
							mate_pop_ptr = &(matepop);
							realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

							/********************************************************************/

							new_pop_ptr = &(newpop);
							mate_pop_ptr = &(matepop);
							func(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
							/********************************************************************/

							/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
							old_pop_ptr = &(oldpop);
							new_pop_ptr = &(newpop);
							mate_pop_ptr = &(matepop);
							keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

							/********************************************************************/
							mate_pop_ptr = &(matepop);
							func(mate_pop_ptr);
							/********************************************************************/

							/*=======Copying the new population to old population======*/
							old_pop_ptr = &(oldpop);
							new_pop_ptr = &(matepop);
							/*	func(new_pop_ptr);
							new_pop_ptr = &(matepop);*/
							for (j = 0; j < popsize; j++)
							{
								old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
								new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
								if (nvar > 0)
								{
									/*For Real Coded GA copying of the chromosomes*/
									for (l = 0; l < nvar; l++)
										old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
								}
								/*Copying the fitness vector */
								for (l = 0; l < nfunc; l++)
									old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

								/*Copying the crowd degree*/
								old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

								/*Copying the rank of the individuals*/
								old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

								/*Copying the flag of the individuals*/
								old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
							}   // end of j

							/*Copying the array having the record of the individual	at different ranks */
							for (l = 0; l < popsize; l++)
							{
								old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
							}
							/*Copying the maxrank */
							old_pop_ptr->maxrank = new_pop_ptr->maxrank;

							/********************************************************************/

							/*Printing the fitness record for last generation in a file last*/


						}// end of i

						//	finish = clock();
						//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
						//	fprintf_s(run_time, "%3.4f ", duration);
						if (nop == noperiods - 1)
						{  // for the last period
							old_pop_ptr = &(oldpop);
							for (f = 0; f < popsize; f++) // for printing
							{
								old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

								// for all feasible solutions and non-dominated solutions
								if (old_pop_ptr->ind_ptr->rank == 1)
								{
									for (l = 0; l < nfunc; l++)
									{
										if (l == 1) {
											fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

										}
										else {
											fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
										}

									}
									fprintf(end_ptr, "%s", "\n");


								}  // feasibility check
							} // end of f (printing)
						}


					}
					fclose(end_ptr);
				}


				//fclose(run_time);

				//	printf_s("please input a char to exit the program!\n");

				//	getchar();

				//return 0;
				MessageBoxW(_T("运行结束"));
				MessageBoxW(_T("将文件内容写入数据库"));
				CString mstrsql;
				mstrsql.Format(_T("CREATE TABLE ged50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
				char *sql = CSt2char(mstrsql);
				if (mysql_real_query(&m_mysql, sql, strlen(sql)))
				{
					MessageBoxW(_T("失败"));
				}
				else
				{
					MessageBoxW(_T("建表成功！"));
				}
				for (int fwj = 0; fwj < 30; fwj++)
				{
					char  mstrsql1[300], mstrsql2[200];
					sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
					CString msq(mstrsql2), msqll1(mstrsql1);
					//sprintf_s(mstrsql1,);
					msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged50(pop,gen)"));
					char *sql2 = CSt2char(msqll1);
					if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
					{
						MessageBoxW(_T("填表失败"));
					}
					else
					{
						if (fwj == 29)
							MessageBoxW(_T("数据库存储数据成功"));
						continue;
					}
				}


			}
			//ged3运行完毕
			CString msql1;
			//msq2.Format(_T())
			msql1.Format(_T("drop table if exists gedir50"));
			char *sql11 = CSt2char(msql1);
			if (mysql_real_query(&m_mysql, sql11, strlen(sql11)))
			{
				MessageBoxW(_T("失败"));
			}
			else
			{
				MessageBoxW(_T("清空表成功！"));
			}
			nvar = maxvar;

			/*number of the functions*/
			nfunc = maxfun1;

			/*number of the individuals in the population*/
			if (zqs == 50)
				popsize = maxpop;
			else
				if (zqs == 100)
					popsize = maxpop1;
				else
					popsize = maxpop2;

			/*No. of generations*/

			gener = MAX_GENERATION;


			int int_exp = 0;

			for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
			{
				printf_s("Run experiment %d!\n", int_exp + 1);

				//	sleep((clock_t)3 * CLOCKS_PER_SEC);

				FILE *end_ptr;
				char file1[500];

				sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\complex\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
				fopen_s(&end_ptr, file1, "w+");


				//clock_t start, finish;
				//	double duration;

				int i, j, l, f;

				population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

				time_stage = T_MAX_RESOURCE;
				//input();
				/*number of the variables*/

				/*the crossover probability (between 0.5 and 1)*/
				pcross = 0.9;

				/*the mutation probability*/
				pmut_r = 0.1;

				/*Distribution Index for real-coded crossover between 0.5 to 100*/
				di = 100;

				/*Distribution Index for real-coded mutation between 0.5 to 500*/
				dim = 500;

				seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
				warmup_random(seed);

				//	start = clock();
				/***********************************************/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);

				for (j = 0; j < popsize; j++)
				{
					/*Initializing the Rank array having different individuals at a particular  rank to zero*/
					old_pop_ptr->rankno[j] = 0;
					new_pop_ptr->rankno[j] = 0;
					mate_pop_ptr->rankno[j] = 0;

					old_pop_ptr->ind[j].cub_len = 0.0;
					new_pop_ptr->ind[j].cub_len = 0.0;
					mate_pop_ptr->ind[j].cub_len = 0.0;
				}
				/*******************************************************/

				old_pop_ptr = &(oldpop);
				realinitir(old_pop_ptr);

				old_pop_ptr = &(oldpop);
				funcir(old_pop_ptr);/*Function Calculation*/

				/********************************************************************/
				/*----------------------GENERATION STARTS HERE----------------------*/
				for (i = 0; i < gener; i++)
				{
					old_pop_ptr = &(oldpop);
					mate_pop_ptr = &(matepop);
					nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

					/********************************************************************/

					new_pop_ptr = &(newpop);
					mate_pop_ptr = &(matepop);
					realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

					/********************************************************************/

					new_pop_ptr = &(newpop);
					real_mutate(new_pop_ptr); /*Real Mutation*/

					new_pop_ptr = &(newpop);
					funcir(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

					/********************************************************************/

					/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
					old_pop_ptr = &(oldpop);
					new_pop_ptr = &(newpop);
					mate_pop_ptr = &(matepop);
					keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

					/********************************************************************/

					/********************************************************************/

					/*=======Copying the new population to old population======*/
					old_pop_ptr = &(oldpop);
					new_pop_ptr = &(matepop);
					for (j = 0; j < popsize; j++)
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
						new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
						if (nvar > 0)
						{
							/*For Real Coded GA copying of the chromosomes*/
							for (l = 0; l < nvar; l++)
								old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
						}

						/*Copying the fitness vector */
						for (l = 0; l < nfunc; l++)
							old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

						/*Copying the crowd degree*/
						old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

						/*Copying the rank of the individuals*/
						old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

						/*Copying the flag of the individuals*/
						old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
					}   // end of j

					/*Copying the array having the record of the individual	at different ranks */
					for (l = 0; l < popsize; l++)
					{
						old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
					}
					/*Copying the maxrank */
					old_pop_ptr->maxrank = new_pop_ptr->maxrank;

					/********************************************************************/

					/*Printing the fitness record for last generation in a file last*/
					if (i == gener - 1)
					{  // for the last generation 
						old_pop_ptr = &(matepop);
						for (f = 0; f < popsize; f++) // for printing
						{
							old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

							// for all feasible solutions and non-dominated solutions
							if (old_pop_ptr->ind_ptr->rank == 1)
							{
								for (l = 0; l < nfunc; l++)
								{
									fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
								}
								fprintf(end_ptr, "%s", "\n");


							}  // feasibility check
						} // end of f (printing)
					}

				}// end of i

				//	finish = clock();
				//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
				//	fprintf_s(run_time, "%3.4f ", duration);

				fclose(end_ptr);
			}

			//fclose(run_time);

			//	printf_s("please input a char to exit the program!\n");

			//	getchar();
			MessageBoxW(_T("运行结束"));
			MessageBoxW(_T("将文件内容写入数据库"));
			CString mstrsql1;
			mstrsql1.Format(_T("CREATE TABLE gedir50(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
			char *sql1 = CSt2char(mstrsql1);
			if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
			{
				MessageBoxW(_T("失败"));
			}
			else
			{
				MessageBoxW(_T("建表成功！"));
			}
			for (int fwj = 0; fwj < 30; fwj++)
			{
				char  mstrsql1[300], mstrsql2[200];
				sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
				CString msq(mstrsql2), msqll1(mstrsql1);
				//sprintf_s(mstrsql1,);
				msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir50(pop,ben,gen)"));
				char *sql2 = CSt2char(msqll1);
				if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
				{
					MessageBoxW(_T("填表失败"));
				}
				else
				{
					if (fwj == 29)
						MessageBoxW(_T("数据库存储数据成功"));
					continue;
				}
			}
			/*********************种群模式下100,250迭代 ***********************************/
			zq100sc0();
			zq250sc0();
		}
		else
		/************************种群模式子系统6*/
		if (zs == 1)
		{
			zq50sc1();
			zq100sc1();
			zq250sc1();
		}
		else
		{
			zq50sc2();
			zq100sc2();
			zq250sc2();
		}


	}
	/**********迭代模式下数据比较，子系统5**************/
	else
		if (gn == 1)
		{
			if (zs == 0)
			{
				gn50sc0();
				gn100sc0();
				gn250sc0();
			}
			else

				if (zs == 1)
				{
					gn50sc1();
					gn100sc1();
					gn250sc1();
				}
				else
				{
					gn50sc2();
					gn100sc2();
					gn250sc2();
				}

		}

}
void CLEFTTOP::OnBnClickedButalog()
{
	
	// TODO:  在此添加控件通知处理程序代码

	Ctestsystem1Doc* tdoc = (Ctestsystem1Doc*)GetDocument();
	tdoc->isOnBnClickedButalogrun = 1;
	//UpdateData(TRUE);
	popn = (((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel());
	gennumber = (((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel());
	alogrithm = m_alogcombox.GetCurSel();
	tdoc->systemnumber = sc;
	
	tdoc->popunum = popn;
	tdoc->generationnumber = gennumber;
	tdoc->butalog = alogrithm;
	CMainFrame* pmain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	rightdownv* rdv = (rightdownv*)(pmain->m_wndSplitter1.GetPane(1, 1));
	tdoc->UpdateAllViews(NULL);
	
	if (m_alogcombox.GetCurSel() == 1)
	{
		
		if ((( CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel()==0)
		{
			CString msql;
			//msq2.Format(_T())
			msql.Format(_T("drop table if exists ged5"));
			char *sql1 = CSt2char(msql);
			if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
			{
				MessageBoxW(_T("失败"));

			}
			else
			{
				MessageBoxW(_T("清空表成功！"));
			}
			ged3();
		MessageBoxW(_T("运行结束"));
		MessageBoxW(_T("将文件内容写入数据库"));
		CString mstrsql;
		mstrsql.Format(_T("CREATE TABLE ged5(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
		char *sql = CSt2char(mstrsql);
		if (mysql_real_query(&m_mysql, sql, strlen(sql)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("建表成功！"));
		}
		for (int fwj = 0; fwj < 30; fwj++)
		{
			char  mstrsql1[300],mstrsql2[200];
			sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"",popsize,gener,fwj+1);
			CString msq(mstrsql2),msqll1(mstrsql1);
			//sprintf_s(mstrsql1,);
			msqll1.Format(_T("load data local infile ")+msq +_T(" into table ged5(pop,gen)"));
			char *sql2 = CSt2char( msqll1);
			if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
			{
				MessageBoxW(_T("填表失败"));
			}
			else
			{
				if (fwj == 29)
					MessageBoxW(_T("数据库存储数据成功"));
				continue;
			}
		}
			//into table person(name, age, city, salary);
		/*****************************************************************************************************************************/
		/*

          运行6个，7个子系统gde3算法；
		*/
	
	
		/*********************************************************************************************************************/
	
	

	/*This also demarcates the different Pareto Fronts*/
	}
	else
			if (((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel() == 1)
			{
				CString msql;
				//msq2.Format(_T())
				msql.Format(_T("drop table if exists ged6"));
				char *sql1 = CSt2char(msql);
				if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
				{
					MessageBoxW(_T("失败"));
				}
				else
				{
					MessageBoxW(_T("清空表成功！"));
				}
				nvar = maxvar1;

				/*number of the functions*/
				nfunc = maxfun;
				if (popn == 0)
				{/*number of the individuals in the population*/
					popsize = maxpop;
				}
				else
					if (popn == 1)
						popsize = maxpop1;
					else
						popsize = maxpop2;

				/*No. of generations*/
				if (gennumber == 0)
					gener = MAX_GENERATION;
				else
					if (gennumber == 1)
						gener = MAX_GENERATION1;
					else
						gener = MAX_GENERATION2;
						nsystem = MAX_SYSTEM1;

				/*Max no. of variables*/

				//#define maxvar    30


				//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
				//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
				//	};//large-scale


				//double para_module[maxvar][2];

				/********************************************************************/


				/*Max value of testing time*/

				//#define T_MAX_RESOURCE 150000.0



				/*Max no. of subsystems*/


				/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







				//void re_parameter(individual *ind, double &cost, int no);
				//void input(); /*Input Parameters from user*/

				//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
				//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
				//void ranking(population *pop_ptr);
				//int indcmp(double *ptr1, double *ptr2);
				//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
				//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
				//void grank(int gen, int y);
				//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

				//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
				//void gshare(int rnk);/*Sharing the fitness*/
				//void sort(int m1);

				//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

				/********************************************************************/

				//int int_exp;

				for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
				{
					printf_s("Run experiment %d!\n", int_exp + 1);

					//	sleep((clock_t)3 * CLOCKS_PER_SEC);

					FILE *end_ptr;
					char file1[500];

					sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods,gener , int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
					fopen_s(&end_ptr, file1, "w+");


					//clock_t start, finish;
					//	double duration;





					//input();

					for (int nop = 0; nop < noperiods; nop++) {
						old_pop_ptr = &(oldpop);
						sumre += tperiod1[nop];
						if (nop > 0) {
							individual *ind = &(old_pop_ptr->ind[0]);
							for (int i = 0; i < popsize; i++) {
								if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
									ind = &(old_pop_ptr->ind[i]);
								}
							}
							costre += ind->fitness[1];
							re_parameter1(ind, costre, nop - 1);

							time_stage = sumre - costre;
						}
						else {
							time_stage = tperiod1[nop];
						}


						seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
						srand((unsigned)time(NULL));
						warmup_random(seed);

						//	start = clock();
						/***********************************************/
						old_pop_ptr = &(oldpop);
						new_pop_ptr = &(newpop);
						mate_pop_ptr = &(matepop);

						for (j = 0; j < popsize; j++)
						{
							/*Initializing the Rank array having different individuals at a particular  rank to zero*/
							old_pop_ptr->rankno[j] = 0;
							new_pop_ptr->rankno[j] = 0;
							mate_pop_ptr->rankno[j] = 0;

							old_pop_ptr->ind[j].cub_len = 0.0;
							new_pop_ptr->ind[j].cub_len = 0.0;
							mate_pop_ptr->ind[j].cub_len = 0.0;
						}
						/*******************************************************/

						old_pop_ptr = &(oldpop);
						realinit(old_pop_ptr);

						old_pop_ptr = &(oldpop);
						func1(old_pop_ptr);	/*Function Calculation*/
						/********************************************************************/
						/*----------------------GENERATION STARTS HERE----------------------*/
						for (i = 0; i < gener; i++)
						{
							old_pop_ptr = &(oldpop);
							mate_pop_ptr = &(matepop);
							nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

							/********************************************************************/
							mate_pop_ptr = &(matepop);
							func1(mate_pop_ptr);
							new_pop_ptr = &(newpop);
							mate_pop_ptr = &(matepop);
							realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

							/********************************************************************/

							new_pop_ptr = &(newpop);
							mate_pop_ptr = &(matepop);
							func1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
							/********************************************************************/

							/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
							old_pop_ptr = &(oldpop);
							new_pop_ptr = &(newpop);
							mate_pop_ptr = &(matepop);
							keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

							/********************************************************************/
							mate_pop_ptr = &(matepop);
							func1(mate_pop_ptr);
							/********************************************************************/

							/*=======Copying the new population to old population======*/
							old_pop_ptr = &(oldpop);
							new_pop_ptr = &(matepop);
							/*	func(new_pop_ptr);
							new_pop_ptr = &(matepop);*/
							for (j = 0; j < popsize; j++)
							{
								old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
								new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
								if (nvar > 0)
								{
									/*For Real Coded GA copying of the chromosomes*/
									for (l = 0; l < nvar; l++)
										old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
								}
								/*Copying the fitness vector */
								for (l = 0; l < nfunc; l++)
									old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

								/*Copying the crowd degree*/
								old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

								/*Copying the rank of the individuals*/
								old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

								/*Copying the flag of the individuals*/
								old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
							}   // end of j

							/*Copying the array having the record of the individual	at different ranks */
							for (l = 0; l < popsize; l++)
							{
								old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
							}
							/*Copying the maxrank */
							old_pop_ptr->maxrank = new_pop_ptr->maxrank;

							/********************************************************************/

							/*Printing the fitness record for last generation in a file last*/


						}// end of i

						//	finish = clock();
						//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
						//	fprintf_s(run_time, "%3.4f ", duration);
						if (nop == noperiods - 1)
						{  // for the last period
							old_pop_ptr = &(oldpop);
							for (f = 0; f < popsize; f++) // for printing
							{
								old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

								// for all feasible solutions and non-dominated solutions
								if (old_pop_ptr->ind_ptr->rank == 1)
								{
									for (l = 0; l < nfunc; l++)
									{
										if (l == 1) {
											fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

										}
										else {
											fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
										}

									}
									fprintf(end_ptr, "%s", "\n");


								}  // feasibility check
							} // end of f (printing)
						}


					}
					fclose(end_ptr);
				}


				//fclose(run_time);

				//	printf_s("please input a char to exit the program!\n");

				//	getchar();

				//return 0;
				MessageBoxW(_T("运行结束"));
				MessageBoxW(_T("将文件内容写入数据库"));
				CString mstrsql;
				mstrsql.Format(_T("CREATE TABLE ged6(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
				char *sql = CSt2char(mstrsql);
				if (mysql_real_query(&m_mysql, sql, strlen(sql)))
				{
					MessageBoxW(_T("失败"));
				}
				else
				{
					MessageBoxW(_T("建表成功！"));
				}
				for (int fwj = 0; fwj < 30; fwj++)
				{
					char  mstrsql1[300], mstrsql2[200];
					sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"",popsize,gener, fwj + 1);
					CString msq(mstrsql2), msqll1(mstrsql1);
					//sprintf_s(mstrsql1,);
					msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged6(pop,gen)"));
					char *sql2 = CSt2char(msqll1);
					if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
					{
						MessageBoxW(_T("填表失败"));
					}
					else
					{
						if (fwj == 29)
							MessageBoxW(_T("数据库存储数据成功"));
						continue;
					}
				}
				//into
		}
		else
		{
			CString msql;
			//msq2.Format(_T())
			msql.Format(_T("drop table if exists ged7"));
			char *sql1 = CSt2char(msql);
			if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
			{
				CWnd::MessageBox(_T("失败"));
			}
			else
			{
				MessageBox(_T("清空表成功！"));
			}
			nvar = maxvar2;

			/*number of the functions*/
			nfunc = maxfun;
			if (popn == 0)
			{/*number of the individuals in the population*/
				popsize = maxpop;
			}
			else
				if (popn == 1)
					popsize = maxpop1;
				else
					popsize = maxpop2;

			/*No. of generations*/
			if (gennumber == 0)
				gener = MAX_GENERATION;
			else
				if (gennumber == 1)
					gener = MAX_GENERATION1;
				else
					gener = MAX_GENERATION2;
			nsystem = MAX_SYSTEM2;

			/*Max no. of variables*/

			//#define maxvar    30


			//double err_detect[2][maxvar] = { 0.942918, 0.836488, 0.857666, 0.677448, 0.634551, 0.881151, 0.718849, 0.646158, 0.657922, 0.596895, 0.691049, 0.940440, 0.835049, 0.674614, 0.797209, 0.571075, 0.594560, 0.611935, 0.560393, 0.620936, 0.765064, 0.858207, 0.793961, 0.799473, 0.669914, 0.855059, 0.904536, 0.591854, 0.571317, 0.629054,
			//	0.969433, 0.975146, 0.912847, 0.961091, 0.854577, 0.992692, 0.923028, 0.830162, 0.751148, 0.917104, 0.997445, 0.993571, 0.911903, 0.782448, 0.923679, 0.952765, 0.773211, 0.857336, 0.829308, 0.857938, 0.819021, 0.753085, 0.957632, 0.747616, 0.895342, 0.913580, 0.964639, 0.957559, 0.883574, 0.761874
			//	};//large-scale


			//double para_module[maxvar][2];

			/********************************************************************/


			/*Max value of testing time*/

			//#define T_MAX_RESOURCE 150000.0



			/*Max no. of subsystems*/


			/*the no. of modules in each subsystems*//*note that the sum is equal to "maxvar"*/







			//void re_parameter(individual *ind, double &cost, int no);
			//void input(); /*Input Parameters from user*/

			//double cal_reliab(int ind_module, double m_res);//calculate the reliability of each modulevoid func(population *pop_ptr)
			//void func(population *pop_ptr);					/*evaluate the value of the function & errors*/
			//void ranking(population *pop_ptr);
			//int indcmp(double *ptr1, double *ptr2);
			//void nselect(population *old_pop_ptr, population *pop2_ptr);/*get the different individuals selected*/
			//void realcross(population *new_pop_ptr, population *mate_pop_ptr);
			//void grank(int gen, int y);
			//int indcmp1(double *ptr1, double *ptr2);/*Comparison of the variables*/

			//void gsort(int rnk, int sel);/*Sorting for the function values in ascending order*/
			//void gshare(int rnk);/*Sharing the fitness*/
			//void sort(int m1);

			//void keepalive(population *pop1_ptr, population *pop2_ptr, population *pop3_ptr, int gen);/*keep the fronts alive (caring the end problem)*/

			/********************************************************************/

			//int int_exp;

			for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
			{
				printf_s("Run experiment %d!\n", int_exp + 1);

				//	sleep((clock_t)3 * CLOCKS_PER_SEC);

				FILE *end_ptr;
				char file1[500];

				sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\15\\larger1\\n=%d\\no=%d\\g=%d\\fitness%d.txt", popsize, noperiods, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
				fopen_s(&end_ptr, file1, "w+");


				//clock_t start, finish;
				//	double duration;





				//input();

				for (int nop = 0; nop < noperiods; nop++) {
					old_pop_ptr = &(oldpop);
					sumre += tperiod2[nop];
					if (nop > 0) {
						individual *ind = &(old_pop_ptr->ind[0]);
						for (int i = 0; i < popsize; i++) {
							if (old_pop_ptr->ind[i].fitness[0] < ind->fitness[0] && old_pop_ptr->ind[i].rank == 1) {
								ind = &(old_pop_ptr->ind[i]);
							}
						}
						costre += ind->fitness[1];
						re_parameter2(ind, costre, nop - 1);

						time_stage = sumre - costre;
					}
					else {
						time_stage = tperiod2[nop];
					}


					seed = (1.0 / (MAX_EXPERIMENT + noperiods + 1))*(int_exp + nop + 1);
					srand((unsigned)time(NULL));
					warmup_random(seed);

					//	start = clock();
					/***********************************************/
					old_pop_ptr = &(oldpop);
					new_pop_ptr = &(newpop);
					mate_pop_ptr = &(matepop);

					for (j = 0; j < popsize; j++)
					{
						/*Initializing the Rank array having different individuals at a particular  rank to zero*/
						old_pop_ptr->rankno[j] = 0;
						new_pop_ptr->rankno[j] = 0;
						mate_pop_ptr->rankno[j] = 0;

						old_pop_ptr->ind[j].cub_len = 0.0;
						new_pop_ptr->ind[j].cub_len = 0.0;
						mate_pop_ptr->ind[j].cub_len = 0.0;
					}
					/*******************************************************/

					old_pop_ptr = &(oldpop);
					realinit(old_pop_ptr);

					old_pop_ptr = &(oldpop);
					func2(old_pop_ptr);	/*Function Calculation*/
					/********************************************************************/
					/*----------------------GENERATION STARTS HERE----------------------*/
					for (i = 0; i < gener; i++)
					{
						old_pop_ptr = &(oldpop);
						mate_pop_ptr = &(matepop);
						nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

						/********************************************************************/
						mate_pop_ptr = &(matepop);
						func2(mate_pop_ptr);
						new_pop_ptr = &(newpop);
						mate_pop_ptr = &(matepop);
						realcross(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

						/********************************************************************/

						new_pop_ptr = &(newpop);
						mate_pop_ptr = &(matepop);
						func2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/
						/********************************************************************/

						/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
						old_pop_ptr = &(oldpop);
						new_pop_ptr = &(newpop);
						mate_pop_ptr = &(matepop);
						keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

						/********************************************************************/
						mate_pop_ptr = &(matepop);
						func2(mate_pop_ptr);
						/********************************************************************/

						/*=======Copying the new population to old population======*/
						old_pop_ptr = &(oldpop);
						new_pop_ptr = &(matepop);
						/*	func(new_pop_ptr);
						new_pop_ptr = &(matepop);*/
						for (j = 0; j < popsize; j++)
						{
							old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
							new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
							if (nvar > 0)
							{
								/*For Real Coded GA copying of the chromosomes*/
								for (l = 0; l < nvar; l++)
									old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
							}
							/*Copying the fitness vector */
							for (l = 0; l < nfunc; l++)
								old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

							/*Copying the crowd degree*/
							old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

							/*Copying the rank of the individuals*/
							old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

							/*Copying the flag of the individuals*/
							old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
						}   // end of j

						/*Copying the array having the record of the individual	at different ranks */
						for (l = 0; l < popsize; l++)
						{
							old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
						}
						/*Copying the maxrank */
						old_pop_ptr->maxrank = new_pop_ptr->maxrank;

						/********************************************************************/

						/*Printing the fitness record for last generation in a file last*/


					}// end of i

					//	finish = clock();
					//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
					//	fprintf_s(run_time, "%3.4f ", duration);
					if (nop == noperiods - 1)
					{  // for the last period
						old_pop_ptr = &(oldpop);
						for (f = 0; f < popsize; f++) // for printing
						{
							old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

							// for all feasible solutions and non-dominated solutions
							if (old_pop_ptr->ind_ptr->rank == 1)
							{
								for (l = 0; l < nfunc; l++)
								{
									if (l == 1) {
										fprintf(end_ptr, "%5.6f\t", (old_pop_ptr->ind_ptr->fitness[l] + costre));

									}
									else {
										fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
									}

								}
								fprintf(end_ptr, "%s", "\n");


							}  // feasibility check
						} // end of f (printing)
					}


				}
				fclose(end_ptr);
			}


			//fclose(run_time);

			//	printf_s("please input a char to exit the program!\n");

			//	getchar();

			//return 0;
			MessageBoxW(_T("运行结束"));
			MessageBoxW(_T("将文件内容写入数据库"));
			CString mstrsql;
			mstrsql.Format(_T("CREATE TABLE ged7(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
			char *sql = CSt2char(mstrsql);
			if (mysql_real_query(&m_mysql, sql, strlen(sql)))
			{
				MessageBoxW(_T("失败"));
			}
			else
			{
				MessageBoxW(_T("建表成功！"));
			}
			for (int fwj = 0; fwj < 30; fwj++)
			{
				char  mstrsql1[300], mstrsql2[200];
				sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/15\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
				CString msq(mstrsql2), msqll1(mstrsql1);
				//sprintf_s(mstrsql1,);
				msqll1.Format(_T("load data local infile ") + msq + _T(" into table ged7(pop,gen)"));
				char *sql2 = CSt2char(msqll1);
				if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
				{
					MessageBoxW(_T("填表失败"));
				}
				else
				{
					if (fwj == 29)
						MessageBoxW(_T("数据库存储数据成功"));
					continue;
				}
			}
		}
}
else
	if (sc == 0)
	{
		CString msql;
		//msq2.Format(_T())
		msql.Format(_T("drop table if exists gedir5"));
		char *sql1 = CSt2char(msql);
		if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("清空表成功！"));
		}
		nvar = maxvar;

		/*number of the functions*/
		nfunc = maxfun1;

		/*number of the individuals in the population*/
		if (popn == 0)
			popsize = maxpop;
		else
			if (popn == 1)
				popsize = maxpop1;
			else
				popsize = maxpop2;

		/*No. of generations*/
		if (gennumber == 0)
			gener = MAX_GENERATION;
		else
			if (gennumber == 1)
				gener = MAX_GENERATION1;
			else
				gener = MAX_GENERATION2;

		int int_exp = 0;

		for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
		{
			printf_s("Run experiment %d!\n", int_exp + 1);

			//	sleep((clock_t)3 * CLOCKS_PER_SEC);

			FILE *end_ptr;
			char file1[500];

			sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\complex\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize,gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
			fopen_s(&end_ptr, file1, "w+");


			//clock_t start, finish;
			//	double duration;

			int i, j, l, f;

			population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

			time_stage = T_MAX_RESOURCE;
			//input();
			/*number of the variables*/
			
			/*the crossover probability (between 0.5 and 1)*/
			pcross = 0.9;

			/*the mutation probability*/
			pmut_r = 0.1;

			/*Distribution Index for real-coded crossover between 0.5 to 100*/
			di = 100;

			/*Distribution Index for real-coded mutation between 0.5 to 500*/
			dim = 500;

			seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
			warmup_random(seed);

			//	start = clock();
			/***********************************************/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			for (j = 0; j < popsize; j++)
			{
				/*Initializing the Rank array having different individuals at a particular  rank to zero*/
				old_pop_ptr->rankno[j] = 0;
				new_pop_ptr->rankno[j] = 0;
				mate_pop_ptr->rankno[j] = 0;

				old_pop_ptr->ind[j].cub_len = 0.0;
				new_pop_ptr->ind[j].cub_len = 0.0;
				mate_pop_ptr->ind[j].cub_len = 0.0;
			}
			/*******************************************************/

			old_pop_ptr = &(oldpop);
			realinitir(old_pop_ptr);

			old_pop_ptr = &(oldpop);
			funcir(old_pop_ptr);/*Function Calculation*/

			/********************************************************************/
			/*----------------------GENERATION STARTS HERE----------------------*/
			for (i = 0; i < gener; i++)
			{
				old_pop_ptr = &(oldpop);
				mate_pop_ptr = &(matepop);
				nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

				/********************************************************************/

				new_pop_ptr = &(newpop);
				real_mutate(new_pop_ptr); /*Real Mutation*/

				new_pop_ptr = &(newpop);
				funcir(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

				/********************************************************************/

				/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);
				keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

				/********************************************************************/

				/********************************************************************/

				/*=======Copying the new population to old population======*/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(matepop);
				for (j = 0; j < popsize; j++)
				{
					old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
					new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
					if (nvar > 0)
					{
						/*For Real Coded GA copying of the chromosomes*/
						for (l = 0; l < nvar; l++)
							old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
					}

					/*Copying the fitness vector */
					for (l = 0; l < nfunc; l++)
						old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

					/*Copying the crowd degree*/
					old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

					/*Copying the rank of the individuals*/
					old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

					/*Copying the flag of the individuals*/
					old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
				}   // end of j

				/*Copying the array having the record of the individual	at different ranks */
				for (l = 0; l < popsize; l++)
				{
					old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
				}
				/*Copying the maxrank */
				old_pop_ptr->maxrank = new_pop_ptr->maxrank;

				/********************************************************************/

				/*Printing the fitness record for last generation in a file last*/
				if (i == gener - 1)
				{  // for the last generation 
					old_pop_ptr = &(matepop);
					for (f = 0; f < popsize; f++) // for printing
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

						// for all feasible solutions and non-dominated solutions
						if (old_pop_ptr->ind_ptr->rank == 1)
						{
							for (l = 0; l < nfunc; l++)
							{
								fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
							}
							fprintf(end_ptr, "%s", "\n");


						}  // feasibility check
					} // end of f (printing)
				}

			}// end of i

			//	finish = clock();
			//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
			//	fprintf_s(run_time, "%3.4f ", duration);

			fclose(end_ptr);
		}

		//fclose(run_time);

		//	printf_s("please input a char to exit the program!\n");

		//	getchar();
		MessageBoxW(_T("运行结束"));
		MessageBoxW(_T("将文件内容写入数据库"));
		CString mstrsql;
		mstrsql.Format(_T("CREATE TABLE gedir5(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
		char *sql = CSt2char(mstrsql);
		if (mysql_real_query(&m_mysql, sql, strlen(sql)))
		{
			MessageBoxW(_T("失败"));
		}
		else
		{
			MessageBoxW(_T("建表成功！"));
		}
		for (int fwj = 0; fwj < 30; fwj++)
		{
			char  mstrsql1[300], mstrsql2[200];
			sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/complex\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
			CString msq(mstrsql2), msqll1(mstrsql1);
			//sprintf_s(mstrsql1,);
			msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir5(pop,ben,gen)"));
			char *sql2 = CSt2char(msqll1);
			if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
			{
				MessageBoxW(_T("填表失败"));
			}
			else
			{
				if (fwj == 29)
					MessageBoxW(_T("数据库存储数据成功"));
				continue;
			}
		}
	}
	else
		if (sc == 1)
		{
			CString msql;
			//msq2.Format(_T())
			msql.Format(_T("drop table if exists gedir6"));
			char *sql1 = CSt2char(msql);
			if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
			{
				MessageBoxW(_T("失败"));
			}
			else
			{
				MessageBoxW(_T("清空表成功！"));
			}
			int int_exp = 0;
			nvar = maxvar1;

			/*number of the functions*/
			nfunc = maxfun1;

			/*number of the individuals in the population*/
			if (popn == 0)
				popsize = maxpop;
			else
				if (popn == 1)
					popsize = maxpop1;
				else
					popsize = maxpop2;

			/*No. of generations*/
			if (gennumber == 0)
				gener = MAX_GENERATION;
			else
				if (gennumber == 1)
					gener = MAX_GENERATION1;
				else
					gener = MAX_GENERATION2;
			for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
			{
				printf_s("Run experiment %d!\n", int_exp + 1);

				//	sleep((clock_t)3 * CLOCKS_PER_SEC);

				FILE *end_ptr;
				char file1[500];

				sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
				fopen_s(&end_ptr, file1, "w+");


				//clock_t start, finish;
				//	double duration;

				int i, j, l, f;

				population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

				time_stage = T_MAX_RESOURCE1;
				//input();
				/*number of the variables*/
				

				/*the crossover probability (between 0.5 and 1)*/
				pcross = 0.9;

				/*the mutation probability*/
				pmut_r = 0.1;

				/*Distribution Index for real-coded crossover between 0.5 to 100*/
				di = 100;

				/*Distribution Index for real-coded mutation between 0.5 to 500*/
				dim = 500;

				seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
				warmup_random(seed);

				//	start = clock();
				/***********************************************/
				old_pop_ptr = &(oldpop);
				new_pop_ptr = &(newpop);
				mate_pop_ptr = &(matepop);

				for (j = 0; j < popsize; j++)
				{
					/*Initializing the Rank array having different individuals at a particular  rank to zero*/
					old_pop_ptr->rankno[j] = 0;
					new_pop_ptr->rankno[j] = 0;
					mate_pop_ptr->rankno[j] = 0;

					old_pop_ptr->ind[j].cub_len = 0.0;
					new_pop_ptr->ind[j].cub_len = 0.0;
					mate_pop_ptr->ind[j].cub_len = 0.0;
				}
				/*******************************************************/

				old_pop_ptr = &(oldpop);
				realinitir(old_pop_ptr);

				old_pop_ptr = &(oldpop);
				funcir1(old_pop_ptr);/*Function Calculation*/

				/********************************************************************/
				/*----------------------GENERATION STARTS HERE----------------------*/
				for (i = 0; i < gener; i++)
				{
					old_pop_ptr = &(oldpop);
					mate_pop_ptr = &(matepop);
					nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

					/********************************************************************/

					new_pop_ptr = &(newpop);
					mate_pop_ptr = &(matepop);
					realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

					/********************************************************************/

					new_pop_ptr = &(newpop);
					real_mutate(new_pop_ptr); /*Real Mutation*/

					new_pop_ptr = &(newpop);
					funcir1(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

					/********************************************************************/

					/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
					old_pop_ptr = &(oldpop);
					new_pop_ptr = &(newpop);
					mate_pop_ptr = &(matepop);
					keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

					/********************************************************************/

					/********************************************************************/

					/*=======Copying the new population to old population======*/
					old_pop_ptr = &(oldpop);
					new_pop_ptr = &(matepop);
					for (j = 0; j < popsize; j++)
					{
						old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
						new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
						if (nvar > 0)
						{
							/*For Real Coded GA copying of the chromosomes*/
							for (l = 0; l < nvar; l++)
								old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
						}

						/*Copying the fitness vector */
						for (l = 0; l < nfunc; l++)
							old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

						/*Copying the crowd degree*/
						old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

						/*Copying the rank of the individuals*/
						old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

						/*Copying the flag of the individuals*/
						old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
					}   // end of j

					/*Copying the array having the record of the individual	at different ranks */
					for (l = 0; l < popsize; l++)
					{
						old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
					}
					/*Copying the maxrank */
					old_pop_ptr->maxrank = new_pop_ptr->maxrank;

					/********************************************************************/

					/*Printing the fitness record for last generation in a file last*/
					if (i == gener - 1)
					{  // for the last generation 
						old_pop_ptr = &(matepop);
						for (f = 0; f < popsize; f++) // for printing
						{
							old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

							// for all feasible solutions and non-dominated solutions
							if (old_pop_ptr->ind_ptr->rank == 1)
							{
								for (l = 0; l < nfunc; l++)
								{
									fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
								}
								fprintf(end_ptr, "%s", "\n");


							}  // feasibility check
						} // end of f (printing)
					}

				}// end of i

				//	finish = clock();
				//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
				//	fprintf_s(run_time, "%3.4f ", duration);

				fclose(end_ptr);
			}

			//fclose(run_time);

			//	printf_s("please input a char to exit the program!\n");

			//	getchar();
			MessageBoxW(_T("运行结束"));
			MessageBoxW(_T("将文件内容写入数据库"));
			CString mstrsql;
			mstrsql.Format(_T("CREATE TABLE gedir6(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
			char *sql = CSt2char(mstrsql);
			if (mysql_real_query(&m_mysql, sql, strlen(sql)))
			{
				MessageBoxW(_T("失败"));
			}
			else
			{
				MessageBoxW(_T("建表成功！"));
			}
			for (int fwj = 0; fwj < 30; fwj++)
			{
				char  mstrsql1[300], mstrsql2[200];
				sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
				CString msq(mstrsql2), msqll1(mstrsql1);
				//sprintf_s(mstrsql1,);
				msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir6(pop,ben,gen)"));
				char *sql2 = CSt2char(msqll1);
				if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
				{
					MessageBoxW(_T("填表失败"));
				}
				else
				{
					if (fwj == 29)
						MessageBoxW(_T("数据库存储数据成功"));
					continue;
				}
			}
		}
		else
			if (sc == 2)
			{
				CString msql;
				//msq2.Format(_T())
				msql.Format(_T("drop table if exists gedir7"));
				char *sql1 = CSt2char(msql);
				if (mysql_real_query(&m_mysql, sql1, strlen(sql1)))
				{
					MessageBoxW(_T("失败"));
				}
				else
				{
					MessageBoxW(_T("清空表成功！"));
				}
				int int_exp = 0;
				nvar = maxvar2;

				/*number of the functions*/
				nfunc = maxfun1;

				/*number of the individuals in the population*/
				if (popn == 0)
					popsize = maxpop;
				else
					if (popn == 1)
						popsize = maxpop1;
					else
						popsize = maxpop2;

				/*No. of generations*/
				if (gennumber == 0)
					gener = MAX_GENERATION;
				else
					if (gennumber == 1)
						gener = MAX_GENERATION1;
					else
						gener = MAX_GENERATION2;
				for (int_exp = 0; int_exp < MAX_EXPERIMENT; int_exp++)
				{
					printf_s("Run experiment %d!\n", int_exp + 1);

					//	sleep((clock_t)3 * CLOCKS_PER_SEC);

					FILE *end_ptr;
					char file1[500];

					sprintf_s(file1, "D:\\Users\\xm\\Desktop\\SGRM_multi\\IR_NSGA_II\\larger1\\n=%d\\no=3\\g=%d\\fitness%d.txt", popsize, gener, int_exp + 1);//在file1中写上每次迭代产生的不同fitness文件的路径
					fopen_s(&end_ptr, file1, "w+");


					//clock_t start, finish;
					//	double duration;

					int i, j, l, f;

					population oldpop, newpop, matepop, *old_pop_ptr, *new_pop_ptr, *mate_pop_ptr;/*Defining the population Structures*/

					time_stage = T_MAX_RESOURCE2;
					//input();
					/*number of the variables*/
					

					/*the crossover probability (between 0.5 and 1)*/
					pcross = 0.9;

					/*the mutation probability*/
					pmut_r = 0.1;

					/*Distribution Index for real-coded crossover between 0.5 to 100*/
					di = 100;

					/*Distribution Index for real-coded mutation between 0.5 to 500*/
					dim = 500;

					seed = (1.0 / (MAX_EXPERIMENT + 1))*(int_exp + 1);
					warmup_random(seed);

					//	start = clock();
					/***********************************************/
					old_pop_ptr = &(oldpop);
					new_pop_ptr = &(newpop);
					mate_pop_ptr = &(matepop);

					for (j = 0; j < popsize; j++)
					{
						/*Initializing the Rank array having different individuals at a particular  rank to zero*/
						old_pop_ptr->rankno[j] = 0;
						new_pop_ptr->rankno[j] = 0;
						mate_pop_ptr->rankno[j] = 0;

						old_pop_ptr->ind[j].cub_len = 0.0;
						new_pop_ptr->ind[j].cub_len = 0.0;
						mate_pop_ptr->ind[j].cub_len = 0.0;
					}
					/*******************************************************/

					old_pop_ptr = &(oldpop);
					realinitir(old_pop_ptr);

					old_pop_ptr = &(oldpop);
					funcir2(old_pop_ptr);/*Function Calculation*/

					/********************************************************************/
					/*----------------------GENERATION STARTS HERE----------------------*/
					for (i = 0; i < gener; i++)
					{
						old_pop_ptr = &(oldpop);
						mate_pop_ptr = &(matepop);
						nselect(old_pop_ptr, mate_pop_ptr);/*--------SELECT----------------*/

						/********************************************************************/

						new_pop_ptr = &(newpop);
						mate_pop_ptr = &(matepop);
						realcrossir(new_pop_ptr, mate_pop_ptr);/*Real Cross-over*/

						/********************************************************************/

						new_pop_ptr = &(newpop);
						real_mutate(new_pop_ptr); /*Real Mutation*/

						new_pop_ptr = &(newpop);
						funcir2(new_pop_ptr);/*----------FUNCTION EVALUATION-----------*/

						/********************************************************************/

						/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
						old_pop_ptr = &(oldpop);
						new_pop_ptr = &(newpop);
						mate_pop_ptr = &(matepop);
						keepaliveir(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);/*Elitism And Sharing Implemented*/

						/********************************************************************/

						/********************************************************************/

						/*=======Copying the new population to old population======*/
						old_pop_ptr = &(oldpop);
						new_pop_ptr = &(matepop);
						for (j = 0; j < popsize; j++)
						{
							old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
							new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
							if (nvar > 0)
							{
								/*For Real Coded GA copying of the chromosomes*/
								for (l = 0; l < nvar; l++)
									old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
							}

							/*Copying the fitness vector */
							for (l = 0; l < nfunc; l++)
								old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

							/*Copying the crowd degree*/
							old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

							/*Copying the rank of the individuals*/
							old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

							/*Copying the flag of the individuals*/
							old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
						}   // end of j

						/*Copying the array having the record of the individual	at different ranks */
						for (l = 0; l < popsize; l++)
						{
							old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
						}
						/*Copying the maxrank */
						old_pop_ptr->maxrank = new_pop_ptr->maxrank;

						/********************************************************************/

						/*Printing the fitness record for last generation in a file last*/
						if (i == gener - 1)
						{  // for the last generation 
							old_pop_ptr = &(matepop);
							for (f = 0; f < popsize; f++) // for printing
							{
								old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

								// for all feasible solutions and non-dominated solutions
								if (old_pop_ptr->ind_ptr->rank == 1)
								{
									for (l = 0; l < nfunc; l++)
									{
										fprintf(end_ptr, "%5.6f\t", old_pop_ptr->ind_ptr->fitness[l]);
									}
									fprintf(end_ptr, "%s", "\n");


								}  // feasibility check
							} // end of f (printing)
						}

					}// end of i

					//	finish = clock();
					//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
					//	fprintf_s(run_time, "%3.4f ", duration);

					fclose(end_ptr);
				}

				//fclose(run_time);

				//	printf_s("please input a char to exit the program!\n");

				//	getchar();
				MessageBoxW(_T("运行结束"));
				MessageBoxW(_T("将文件内容写入数据库"));
				CString mstrsql;
				mstrsql.Format(_T("CREATE TABLE gedir7(pop varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,ben varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,gen  varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL) ENGINE = InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = COMPACT"));
				char *sql = CSt2char(mstrsql);
				if (mysql_real_query(&m_mysql, sql, strlen(sql)))
				{
					MessageBoxW(_T("失败"));
				}
				else
				{
					MessageBoxW(_T("建表成功！"));
				}
				for (int fwj = 0; fwj < 30; fwj++)
				{
					char  mstrsql1[300], mstrsql2[200];
					sprintf_s(mstrsql2, "\"D:\/Users\/xm\/Desktop\/SGRM_multi\/IR_NSGA_II\/larger1\/n=%d\/no=3\/g=%d\/fitness%d.txt\"", popsize, gener, fwj + 1);
					CString msq(mstrsql2), msqll1(mstrsql1);
					//sprintf_s(mstrsql1,);
					msqll1.Format(_T("load data local infile ") + msq + _T(" into table gedir7(pop,ben,gen)"));
					char *sql2 = CSt2char(msqll1);
					if (mysql_real_query(&m_mysql, sql2, strlen(sql2)))
					{
						MessageBoxW(_T("填表失败"));
					}
					else
					{
						if (fwj == 29)
							MessageBoxW(_T("数据库存储数据成功"));
						continue;
					}
				}
			}
			
		//	rdv->OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
			//butalog=
			tdoc->isOnBnClickedButalogrun = 0;
			tdoc->UpdateAllViews(NULL);
			

};




//void CLEFTTOP::OnCbnSelchangeCombo1()
//{
//	// TODO:  在此添加控件通知处理程序代码
//
//}


void CLEFTTOP::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	CRect rect;
	CLEFTTOP lft;
	GetWindowRect(&rect);
	MoveWindow(&rect,TRUE);
	m_alogcombox.AddString(_T("IR-NSGR-II"));
	m_alogcombox.AddString(_T("GDE3"));
	m_alogcombox.SetCurSel(0);
	pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO1));
	pCoboBox->AddString(_T("5"));
	pCoboBox->AddString(_T("6"));
	pCoboBox->AddString(_T("7"));
	pCoboBox->SetCurSel(0);
	pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO3));
	pCoboBox->AddString(_T("50"));
	pCoboBox->AddString(_T("100"));
	pCoboBox->AddString(_T("250"));
	pCoboBox->SetCurSel(0);
	pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO4));
	pCoboBox->AddString(_T("50"));
	pCoboBox->AddString(_T("100"));
	pCoboBox->AddString(_T("250"));
	pCoboBox->SetCurSel(0);
	m_edit1.SetWindowText(_T("1"));
	m_edit2.SetWindowText(_T("1"));
	m_edit3.SetWindowText(_T("4"));
	m_edit4.SetWindowText(_T("2"));
	m_edit5.SetWindowText(_T("2"));
	timelimit.SetWindowText(_T("50000.0"));
	time0.SetWindowText(_T("50.0"));
	//	m_edit6.SetWindowText(_T("1"));
	//m_edit7.SetWindowText(_T("1"));
	Ctestsystem1Doc *tdoc = (Ctestsystem1Doc*)GetDocument();
	tdoc->systemnumber = sc;
	tdoc->popunum = popn;
	tdoc->generationnumber = gennumber;
	tdoc->butalog = alogrithm;
	tdoc->UpdateAllViews(NULL);
}


//void CLEFTTOP::OnCbnSelchangeCombo1()
//{
//	// TODO:  在此添加控件通知处理程序代码
//}


void CLEFTTOP::OnCbnSelchangeCombo1()
{
	
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Ctestsystem1Doc *tdoc = (Ctestsystem1Doc*)GetDocument();
	sc = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	zs = sc;
	tdoc->systemnumber = sc;
	tdoc->UpdateAllViews(NULL);
	//if (((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel()==0)
	if (sc==0)
	{
		timelimit.SetWindowText(_T("50000.0"));
		time0.SetWindowText(_T("50.0"));
		m_edit1.SetWindowText(_T("1"));
	    m_edit2.SetWindowText(_T("1"));
		m_edit3.SetWindowText(_T("4"));
		m_edit4.SetWindowText(_T("2"));
		m_edit5.SetWindowText(_T("2"));
	//	m_edit6.SetWindowText(_T("1"));
		//m_edit7.SetWindowText(_T("1"));
		pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO3));
		pCoboBox->SetCurSel(0);
		pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO4));
		pCoboBox->SetCurSel(0);

	}
	else
	{
		//if (((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel() == 1)
		if (sc==1)
		{
			timelimit.SetWindowText(_T("70000.0"));
			time0.SetWindowText(_T("100.0"));
			m_edit1.SetWindowText(_T("1"));
			m_edit2.SetWindowText(_T("2"));
			m_edit3.SetWindowText(_T("4"));
			m_edit4.SetWindowText(_T("3"));
			m_edit5.SetWindowText(_T("3"));
				m_edit6.SetWindowText(_T("1"));
			//m_edit7.SetWindowText(_T("1"));
			pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO3));
			pCoboBox->SetCurSel(1);
			pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO4));
			pCoboBox->SetCurSel(1);
		}
		else
		{
			timelimit.SetWindowText(_T("90000.0"));
			time0.SetWindowText(_T("200.0"));
			m_edit1.SetWindowText(_T("1"));
			m_edit2.SetWindowText(_T("3"));
			m_edit3.SetWindowText(_T("4"));
			m_edit4.SetWindowText(_T("4"));
			m_edit5.SetWindowText(_T("4"));
			m_edit6.SetWindowText(_T("3"));
			m_edit7.SetWindowText(_T("1"));
			pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO3));
			pCoboBox->SetCurSel(2);
			pCoboBox = ((CComboBox*)GetDlgItem(IDC_COMBO4));
			pCoboBox->SetCurSel(2);
		}
		
	}
}

int clik = 0;
void CLEFTTOP::OnBnClickedButton3()
{
	Ctestsystem1Doc* tdoc = (Ctestsystem1Doc*)GetDocument();
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	clik++;
	if ((clik % 2) == 0)
		bdu = 0;
	else
		bdu = 1;
	tdoc->isduibirun = bdu;
	tdoc->UpdateAllViews(NULL);
	// bdu =1;
	 return ;
}


//void CLEFTTOP::OnBnDoubleclickedButton3()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	
//	return;
//}


void CLEFTTOP::OnCbnSelchangeCombo2()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	Ctestsystem1Doc*tdoc = (Ctestsystem1Doc*)GetDocument();
	alogrithm = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
	tdoc->butalog = alogrithm;
	tdoc->UpdateAllViews(NULL);
	return;
}

int clickmodel = 0;
void CLEFTTOP::OnBnClickedButton1()
{
	UpdateData(true);
	Ctestsystem1Doc*tdoc = (Ctestsystem1Doc*)GetDocument();
	clickmodel++;
	if (clickmodel % 2 == 0)
		tdoc->issymod = 0;
	else
		tdoc->issymod = 1;
	
	tdoc->UpdateAllViews(NULL);
	// TODO:  在此添加控件通知处理程序代码
}




// CtestsystemApp




// CtestsystemApp 构造



void CLEFTTOP::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，

		datalist dlg;
		//m_pMainWnd = &dlg;
		 dlg.DoModal();
		
		//dlg.OnInitDialog();
	
	//}

	// 删除上面创建的 shell 管理器。
	
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	//return FALSE;

}
