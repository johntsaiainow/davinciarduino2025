#include <Arduino.h>
#include<temp_read.h>

#define read_number 200
#define ch_number 5
//unsigned int AD_Value[read_number][ch_number];

unsigned int ADC_RD(unsigned char u8_channel)
{
  unsigned int i;

  unsigned long sum=0;

  for (i=0;i<read_number;i++)
  {
    analogReadResolution(12);
    int value = analogRead(u8_channel);
    // Serial2.print("ANALOG READING: ");
    //Serial2.println(value);
    sum+=value;
  }
  sum=sum/(read_number/4);

  return (unsigned int)sum;
}

unsigned int maxminaverage(unsigned int *p,unsigned char len,unsigned char maxmin_len)
{
	unsigned char i,n,a;
	unsigned long j;
	unsigned long m;

//排序	
	a=len;
	len--;
	do{
		n=0;
		for(i=0;i<len;i++)
		{
			if(p[i]>p[i+1])
			{
				j=p[i];
				p[i]=p[i+1];
				p[i+1]=j;
				n=1;
			}
		}
	}
	while(n);
	
	
//去除最大值最小值求平均值
	m=0;
	for(i=maxmin_len/2;i<(a-maxmin_len/2);i++)
	{
		m+=p[i];
	}

	j=m/(a-maxmin_len);

	return j;

}


const unsigned int tem_dat_cold[141]=
{
1884,          //0 [-20癈]
1969,          //1 [-19癈]
2056,          //2 [-18癈]
2147,          //3 [-17癈]
2240,          //4 [-16癈]
2336,          //5 [-15癈]
2435,          //6 [-14癈]
2536,          //7 [-13癈]
2640,          //8 [-12癈]
2747,          //9 [-11癈]
2857,          //10 [-10癈]
2969,          //11 [-9癈]
3084,          //12 [-8癈]
3202,          //13 [-7癈]
3322,          //14 [-6癈]
3445,          //15 [-5癈]
3570,          //16 [-4癈]
3698,          //17 [-3癈]
3828,          //18 [-2癈]
3961,          //19 [-1癈]
4096,          //20 [0癈]
4232,          //21 [1癈]
4371,          //22 [2癈]
4512,          //23 [3癈]
4656,          //24 [4癈]
4800,          //25 [5癈]
4947,          //26 [6癈]
5095,          //27 [7癈]
5245,          //28 [8癈]
5395,          //29 [9癈]
5548,          //30 [10癈]
5701,          //31 [11癈]
5856,          //32 [12癈]
6010,          //33 [13癈]
6166,          //34 [14癈]
6323,          //35 [15癈]
6481,          //36 [16癈]
6639,          //37 [17癈]
6796,          //38 [18癈]
6954,          //39 [19癈]
7114,          //40 [20癈]
7272,          //41 [21癈]
7427,          //42 [22癈]
7585,          //43 [23癈]
7743,          //44 [24癈]
7900,          //45 [25癈]
8055,          //46 [26癈]
8210,          //47 [27癈]
8365,          //48 [28癈]
8518,          //49 [29癈]
8670,          //50 [30癈]
8821,          //51 [31癈]
8971,          //52 [32癈]
9119,          //53 [33癈]
9265,          //54 [34癈]
9411,          //55 [35癈]
9554,          //56 [36癈]
9697,          //57 [37癈]
9837,          //58 [38癈]
9976,          //59 [39癈]
10113,          //60 [40癈]
10248,          //61 [41癈]
10381,          //62 [42癈]
10512,          //63 [43癈]
10641,          //64 [44癈]
10768,          //65 [45癈]
10893,          //66 [46癈]
11016,          //67 [47癈]
11137,          //68 [48癈]
11256,          //69 [49癈]
11373,          //70 [50癈]
11373,          //71 [51癈]
11373,          //72 [52癈]
11373,          //73 [53癈]
11373,          //74 [54癈]
11373,          //75 [55癈]
11373,          //76 [56癈]
11373,          //77 [57癈]
11373,          //78 [58癈]
11373,          //79 [59癈]
11373,          //80 [60癈]
11373,          //81 [61癈]
11373,          //82 [62癈]
11373,          //83 [63癈]
11373,          //84 [64癈]
11373,          //85 [65癈]
11373,          //86 [66癈]
11373,          //87 [67癈]
11373,          //88 [68癈]
11373,          //89 [69癈]
11373,          //90 [70癈]
11373,          //91 [71癈]
11373,          //92 [72癈]
11373,          //93 [73癈]
11373,          //94 [74癈]
11373,          //95 [75癈]
11373,          //96 [76癈]
11373,          //97 [77癈]
11373,          //98 [78癈]
11373,          //99 [79癈]
11373,          //100 [80癈]
11373,          //101 [81癈]
11373,          //102 [82癈]
11373,          //103 [83癈]
11373,          //104 [84癈]
11373,          //105 [85癈]
11373,          //106 [86癈]
11373,          //107 [87癈]
11373,          //108 [88癈]
11373,          //109 [89癈]
11373,          //110 [90癈]
11373,          //111 [91癈]
11373,          //112 [92癈]
11373,          //113 [93癈]
11373,          //114 [94癈]
11373,          //115 [95癈]
11373,          //116 [96癈]
11373,          //117 [97癈]
11373,          //118 [98癈]
11373,          //119 [99癈]
11373,          //120 [100癈]
11373,          //121 [101癈]
11373,          //122 [102癈]
11373,          //123 [103癈]
11373,          //124 [104癈]
11373,          //125 [105癈]
11373,          //126 [106癈]
11373,          //127 [107癈]
11373,          //128 [108癈]
11373,          //129 [109癈]
11373,          //130 [110癈]
11373,          //131 [111癈]
11373,          //132 [112癈]
11373,          //133 [113癈]
11373,          //134 [114癈]
11373,          //135 [115癈]
11373,          //136 [116癈]
11373,          //137 [117癈]
11373,          //138 [118癈]
11373,          //139 [119癈]
11373,          //140 [120癈]
};
const unsigned int tem_dat_hot[141]=
{
165,          //0 [-20癈]
175,          //1 [-19癈]
185,          //2 [-18癈]
196,          //3 [-17癈]
207,          //4 [-16癈]
219,          //5 [-15癈]
232,          //6 [-14癈]
245,          //7 [-13癈]
259,          //8 [-12癈]
273,          //9 [-11癈]
288,          //10 [-10癈]
304,          //11 [-9癈]
320,          //12 [-8癈]
338,          //13 [-7癈]
356,          //14 [-6癈]
375,          //15 [-5癈]
395,          //16 [-4癈]
415,          //17 [-3癈]
437,          //18 [-2癈]
459,          //19 [-1癈]
483,          //20 [0癈]
508,          //21 [1癈]
534,          //22 [2癈]
561,          //23 [3癈]
589,          //24 [4癈]
618,          //25 [5癈]
649,          //26 [6癈]
681,          //27 [7癈]
714,          //28 [8癈]
748,          //29 [9癈]
784,          //30 [10癈]
822,          //31 [11癈]
860,          //32 [12癈]
900,          //33 [13癈]
942,          //34 [14癈]
985,          //35 [15癈]
1030,          //36 [16癈]
1076,          //37 [17癈]
1124,          //38 [18癈]
1174,          //39 [19癈]
1225,          //40 [20癈]
1278,          //41 [21癈]
1333,          //42 [22癈]
1390,          //43 [23癈]
1447,          //44 [24癈]
1508,          //45 [25癈]
1570,          //46 [26癈]
1634,          //47 [27癈]
1700,          //48 [28癈]
1768,          //49 [29癈]
1838,          //50 [30癈]
1910,          //51 [31癈]
1984,          //52 [32癈]
2060,          //53 [33癈]
2138,          //54 [34癈]
2218,          //55 [35癈]
2300,          //56 [36癈]
2384,          //57 [37癈]
2470,          //58 [38癈]
2558,          //59 [39癈]
2648,          //60 [40癈]
2740,          //61 [41癈]
2834,          //62 [42癈]
2930,          //63 [43癈]
3028,          //64 [44癈]
3129,          //65 [45癈]
3231,          //66 [46癈]
3335,          //67 [47癈]
3438,          //68 [48癈]
3549,          //69 [49癈]
3658,          //70 [50癈]
3769,          //71 [51癈]
3882,          //72 [52癈]
3997,          //73 [53癈]
4113,          //74 [54癈]
4231,          //75 [55癈]
4351,          //76 [56癈]
4472,          //77 [57癈]
4595,          //78 [58癈]
4718,          //79 [59癈]
4843,          //80 [60癈]
4969,          //81 [61癈]
5096,          //82 [62癈]
5224,          //83 [63癈]
5354,          //84 [64癈]
5485,          //85 [65癈]
5617,          //86 [66癈]
5749,          //87 [67癈]
5881,          //88 [68癈]
6015,          //89 [69癈]
6150,          //90 [70癈]
6285,          //91 [71癈]
6418,          //92 [72癈]
6554,          //93 [73癈]
6690,          //94 [74癈]
6827,          //95 [75癈]
6960,          //96 [76癈]
7096,          //97 [77癈]
7234,          //98 [78癈]
7367,          //99 [79癈]
7505,          //100 [80癈]
7638,          //101 [81癈]
7772,          //102 [82癈]
7907,          //103 [83癈]
8043,          //104 [84癈]
8176,          //105 [85癈]
8307,          //106 [86癈]
8439,          //107 [87癈]
8570,          //108 [88癈]
8700,          //109 [89癈]
8829,          //110 [90癈]
8958,          //111 [91癈]
9085,          //112 [92癈]
9212,          //113 [93癈]
9337,          //114 [94癈]
9461,          //115 [95癈]
9584,          //116 [96癈]
9706,          //117 [97癈]
9825,          //118 [98癈]
9945,          //119 [99癈]
10063,          //120 [100癈]
10179,          //121 [101癈]
10294,          //122 [102癈]
10408,          //123 [103癈]
10520,          //124 [104癈]
10631,          //125 [105癈]
10740,          //126 [106癈]
10847,          //127 [107癈]
10953,          //128 [108癈]
11058,          //129 [109癈]
11162,          //130 [110癈]
11263,          //131 [111癈]
11363,          //132 [112癈]
11461,          //133 [113癈]
11558,          //134 [114癈]
11654,          //135 [115癈]
11748,          //136 [116癈]
11840,          //137 [117癈]
11930,          //138 [118癈]
12020,          //139 [119癈]
12108,          //140 [120癈]
};

const unsigned int tem_dat[141]=
{
2054,          //0 [-20°C]
2144,          //1 [-19°C]
2237,          //2 [-18°C]
2333,          //3 [-17°C]
2431,          //4 [-16°C]
2557,          //5 [-15°C]
2636,          //6 [-14°C]
2743,          //7 [-13°C]
2852,          //8 [-12°C]
2964,          //9 [-11°C]
3079,          //10 [-10°C]
3196,          //11 [-9°C]
3317,          //12 [-8°C]
3439,          //13 [-7°C]
3564,          //14 [-6°C]
3692,          //15 [-5°C]
3821,          //16 [-4°C]
3954,          //17 [-3°C]
4088,          //18 [-2°C]
4225,          //19 [-1°C]
4363,          //20 [0°C]
4504,          //21 [1°C]
4647,          //22 [2°C]
4791,          //23 [3°C]
4936,          //24 [4°C]
5085,          //25 [5°C]
5233,          //26 [6°C]
5384,          //27 [7°C]
5535,          //28 [8°C]
5689,          //29 [9°C]
5843,          //30 [10°C]
5997,          //31 [11°C]
6152,          //32 [12°C]
6309,          //33 [13°C]
6466,          //34 [14°C]
6622,          //35 [15°C]
6781,          //36 [16°C]
6939,          //37 [17°C]
7096,          //38 [18°C]
7253,          //39 [19°C]
7410,          //40 [20°C]
7568,          //41 [21°C]
7725,          //42 [22°C]
7881,          //43 [23°C]
8039,          //44 [24°C]
8192,          //45 [25°C]
8346,          //46 [26°C]
8499,          //47 [27°C]
8650,          //48 [28°C]
8801,          //49 [29°C]
8951,          //50 [30°C]
9099,          //51 [31°C]
9245,          //52 [32°C]
9390,          //53 [33°C]
9534,          //54 [34°C]
9676,          //55 [35°C]
9816,          //56 [36°C]
9954,          //57 [37°C]
10091,          //58 [38°C]
10226,          //59 [39°C]
10359,          //60 [40°C]
10490,          //61 [41°C]
10619,          //62 [42°C]
10746,          //63 [43°C]
10871,          //64 [44°C]
10994,          //65 [45°C]
11115,          //66 [46°C]
11234,          //67 [47°C]
11351,          //68 [48°C]
11465,          //69 [49°C]
11578,          //70 [50°C]
11689,          //71 [51°C]
11797,          //72 [52°C]
11903,          //73 [53°C]
12007,          //74 [54°C]
12109,          //75 [55°C]
12210,          //76 [56°C]
12307,          //77 [57°C]
12403,          //78 [58°C]
12497,          //79 [59°C]
12589,          //80 [60°C]
12678,          //81 [61°C]
12766,          //82 [62°C]
12852,          //83 [63°C]
12936,          //84 [64°C]
13019,          //85 [65°C]
13099,          //86 [66°C]
13177,          //87 [67°C]
13254,          //88 [68°C]
13329,          //89 [69°C]
13402,          //90 [70°C]
13473,          //91 [71°C]
13543,          //92 [72°C]
13610,          //93 [73°C]
13677,          //94 [74°C]
13742,          //95 [75°C]
13805,          //96 [76°C]
13866,          //97 [77°C]
13926,          //98 [78°C]
13985,          //99 [79°C]
14042,          //100 [80°C]
14097,          //101 [81°C]
14152,          //102 [82°C]
14205,          //103 [83°C]
14257,          //104 [84°C]
14308,          //105 [85°C]
14357,          //106 [86°C]
14405,          //107 [87°C]
14452,          //108 [88°C]
14497,          //109 [89°C]
14542,          //110 [90°C]
14584,          //111 [91°C]
14626,          //112 [92°C]
14668,          //113 [93°C]
14707,          //114 [94°C]
14747,          //115 [95°C]
14784,          //116 [96°C]
14822,          //117 [97°C]
14858,          //118 [98°C]
14893,          //119 [99°C]
14927,          //120 [100°C]
14961,          //121 [101°C]
14993,          //122 [102°C]
15025,          //123 [103°C]
15056,          //124 [104°C]
15086,          //125 [105°C]
15116,          //126 [106°C]
15144,          //127 [107°C]
15172,          //128 [108°C]
15199,          //129 [109°C]
15226,          //130 [110°C]
15252,          //131 [111°C]
15277,          //132 [112°C]
15302,          //133 [113°C]
15326,          //134 [114°C]
15349,          //135 [115°C]
15372,          //136 [116°C]
15394,          //137 [117°C]
15415,          //138 [118°C]
15437,          //139 [119°C]
15457,          //140 [120°C]
};



int temp_change(unsigned int val)
{
    int dd;
	//int ad_value_temp;
    //val=val-32768;
    //---------------------------------------------------------------------------------

	if(val<tem_dat[0])dd=-10000;
	else if(val>=tem_dat[0]&&val<(tem_dat[1]))dd=((val-tem_dat[0])*100)/(tem_dat[1]-tem_dat[0])-2000;
    else if(val>=tem_dat[1]&&val<(tem_dat[2]))dd=((val-tem_dat[1])*100)/(tem_dat[2]-tem_dat[1])-1900;
    else if(val>=tem_dat[2]&&val<(tem_dat[3]))dd=((val-tem_dat[2])*100)/(tem_dat[3]-tem_dat[2])-1800;
    else if(val>=tem_dat[3]&&val<(tem_dat[4]))dd=((val-tem_dat[3])*100)/(tem_dat[4]-tem_dat[3])-1700;
    else if(val>=tem_dat[4]&&val<(tem_dat[5]))dd=((val-tem_dat[4])*100)/(tem_dat[5]-tem_dat[4])-1600;
    else if(val>=tem_dat[5]&&val<(tem_dat[6]))dd=((val-tem_dat[5])*100)/(tem_dat[6]-tem_dat[5])-1500;
    else if(val>=tem_dat[6]&&val<(tem_dat[7]))dd=((val-tem_dat[6])*100)/(tem_dat[7]-tem_dat[6])-1400;
    else if(val>=tem_dat[7]&&val<(tem_dat[8]))dd=((val-tem_dat[7])*100)/(tem_dat[8]-tem_dat[7])-1300;
    else if(val>=tem_dat[8]&&val<(tem_dat[9]))dd=((val-tem_dat[8])*100)/(tem_dat[9]-tem_dat[8])-1200;
    else if(val>=tem_dat[9]&&val<(tem_dat[10]))dd=((val-tem_dat[9])*100)/(tem_dat[10]-tem_dat[9])-1100;
	else if(val>=tem_dat[10]&&val<(tem_dat[11]))dd=((val-tem_dat[10])*100)/(tem_dat[11]-tem_dat[10])-1000;
   
   	else if(val>=tem_dat[11]&&val<(tem_dat[12]))dd=((val-tem_dat[11])*100)/(tem_dat[12]-tem_dat[11])-900;
    else if(val>=tem_dat[12]&&val<(tem_dat[13]))dd=((val-tem_dat[12])*100)/(tem_dat[13]-tem_dat[12])-800;
    else if(val>=tem_dat[13]&&val<(tem_dat[14]))dd=((val-tem_dat[13])*100)/(tem_dat[14]-tem_dat[13])-700;
    else if(val>=tem_dat[14]&&val<(tem_dat[15]))dd=((val-tem_dat[14])*100)/(tem_dat[15]-tem_dat[14])-600;
    else if(val>=tem_dat[15]&&val<(tem_dat[16]))dd=((val-tem_dat[15])*100)/(tem_dat[16]-tem_dat[15])-500;
    else if(val>=tem_dat[16]&&val<(tem_dat[17]))dd=((val-tem_dat[16])*100)/(tem_dat[17]-tem_dat[16])-400;
    else if(val>=tem_dat[17]&&val<(tem_dat[18]))dd=((val-tem_dat[17])*100)/(tem_dat[18]-tem_dat[17])-300;
    else if(val>=tem_dat[18]&&val<(tem_dat[19]))dd=((val-tem_dat[18])*100)/(tem_dat[19]-tem_dat[18])-200;
    else if(val>=tem_dat[19]&&val<(tem_dat[20]))dd=((val-tem_dat[19])*100)/(tem_dat[20]-tem_dat[19])-100;
	else if(val>=tem_dat[20]&&val<(tem_dat[21]))dd=0+((val-tem_dat[20])*100)/(tem_dat[21]-tem_dat[20]);
	
	else if(val>=tem_dat[21]&&val<(tem_dat[22]))dd=100+((val-tem_dat[21])*100)/(tem_dat[22]-tem_dat[21]);
    else if(val>=tem_dat[22]&&val<(tem_dat[23]))dd=200+((val-tem_dat[22])*100)/(tem_dat[23]-tem_dat[22]);
    else if(val>=tem_dat[23]&&val<(tem_dat[24]))dd=300+((val-tem_dat[23])*100)/(tem_dat[24]-tem_dat[23]);
    else if(val>=tem_dat[24]&&val<(tem_dat[25]))dd=400+((val-tem_dat[24])*100)/(tem_dat[25]-tem_dat[24]);
    else if(val>=tem_dat[25]&&val<(tem_dat[26]))dd=500+((val-tem_dat[25])*100)/(tem_dat[26]-tem_dat[25]);
    else if(val>=tem_dat[26]&&val<(tem_dat[27]))dd=600+((val-tem_dat[26])*100)/(tem_dat[27]-tem_dat[26]);
    else if(val>=tem_dat[27]&&val<(tem_dat[28]))dd=700+((val-tem_dat[27])*100)/(tem_dat[28]-tem_dat[27]);
    else if(val>=tem_dat[28]&&val<(tem_dat[29]))dd=800+((val-tem_dat[28])*100)/(tem_dat[29]-tem_dat[28]);
    else if(val>=tem_dat[29]&&val<(tem_dat[30]))dd=900+((val-tem_dat[29])*100)/(tem_dat[30]-tem_dat[29]);
	else if(val>=tem_dat[30]&&val<(tem_dat[31]))dd=1000+((val-tem_dat[30])*100)/(tem_dat[31]-tem_dat[30]);

	else if(val>=tem_dat[31]&&val<(tem_dat[32]))dd=1100+((val-tem_dat[31])*100)/(tem_dat[32]-tem_dat[31]);
    else if(val>=tem_dat[32]&&val<(tem_dat[33]))dd=1200+((val-tem_dat[32])*100)/(tem_dat[33]-tem_dat[32]);
    else if(val>=tem_dat[33]&&val<(tem_dat[34]))dd=1300+((val-tem_dat[33])*100)/(tem_dat[34]-tem_dat[33]);
    else if(val>=tem_dat[34]&&val<(tem_dat[35]))dd=1400+((val-tem_dat[34])*100)/(tem_dat[35]-tem_dat[34]);
    else if(val>=tem_dat[35]&&val<(tem_dat[36]))dd=1500+((val-tem_dat[35])*100)/(tem_dat[36]-tem_dat[35]);
    else if(val>=tem_dat[36]&&val<(tem_dat[37]))dd=1600+((val-tem_dat[36])*100)/(tem_dat[37]-tem_dat[36]);
    else if(val>=tem_dat[37]&&val<(tem_dat[38]))dd=1700+((val-tem_dat[37])*100)/(tem_dat[38]-tem_dat[37]);
    else if(val>=tem_dat[38]&&val<(tem_dat[39]))dd=1800+((val-tem_dat[38])*100)/(tem_dat[39]-tem_dat[38]);
    else if(val>=tem_dat[39]&&val<(tem_dat[40]))dd=1900+((val-tem_dat[39])*100)/(tem_dat[40]-tem_dat[39]);
	else if(val>=tem_dat[40]&&val<(tem_dat[41]))dd=2000+((val-tem_dat[40])*100)/(tem_dat[41]-tem_dat[40]);

   //----********21~30**变化量是10/2***********
    else if(val>=tem_dat[41]&&val<(tem_dat[42]))dd=2100+((val-tem_dat[41])*100)/(tem_dat[42]-tem_dat[41]);
    else if(val>=tem_dat[42]&&val<(tem_dat[43]))dd=2200+((val-tem_dat[42])*100)/(tem_dat[43]-tem_dat[42]);
    else if(val>=tem_dat[43]&&val<(tem_dat[44]))dd=2300+((val-tem_dat[43])*100)/(tem_dat[44]-tem_dat[43]);
    else if(val>=tem_dat[44]&&val<(tem_dat[45]))dd=2400+((val-tem_dat[44])*100)/(tem_dat[45]-tem_dat[44]);
    else if(val>=tem_dat[45]&&val<(tem_dat[46]))dd=2500+((val-tem_dat[45])*100)/(tem_dat[46]-tem_dat[45]);
    else if(val>=tem_dat[46]&&val<(tem_dat[47]))dd=2600+((val-tem_dat[46])*100)/(tem_dat[47]-tem_dat[46]);
    else if(val>=tem_dat[47]&&val<(tem_dat[48]))dd=2700+((val-tem_dat[47])*100)/(tem_dat[48]-tem_dat[47]);
    else if(val>=tem_dat[48]&&val<(tem_dat[49]))dd=2800+((val-tem_dat[48])*100)/(tem_dat[49]-tem_dat[48]);
    else if(val>=tem_dat[49]&&val<(tem_dat[50]))dd=2900+((val-tem_dat[49])*100)/(tem_dat[50]-tem_dat[49]);
	else if(val>=tem_dat[50]&&val<(tem_dat[51]))dd=3000+((val-tem_dat[50])*100)/(tem_dat[51]-tem_dat[50]);
    
    //*************31~40**变化量是9/2***********
    else if(val>=tem_dat[51]&&val<(tem_dat[52]))dd=3100+((val-tem_dat[51])*100)/(tem_dat[52]-tem_dat[51]);
    else if(val>=tem_dat[52]&&val<(tem_dat[53]))dd=3200+((val-tem_dat[52])*100)/(tem_dat[53]-tem_dat[52]);
    else if(val>=tem_dat[53]&&val<(tem_dat[54]))dd=3300+((val-tem_dat[53])*100)/(tem_dat[54]-tem_dat[53]);
    else if(val>=tem_dat[54]&&val<(tem_dat[55]))dd=3400+((val-tem_dat[54])*100)/(tem_dat[55]-tem_dat[54]);
    else if(val>=tem_dat[55]&&val<(tem_dat[56]))dd=3500+((val-tem_dat[55])*100)/(tem_dat[56]-tem_dat[55]);
    else if(val>=tem_dat[56]&&val<(tem_dat[57]))dd=3600+((val-tem_dat[56])*100)/(tem_dat[57]-tem_dat[56]);
    else if(val>=tem_dat[57]&&val<(tem_dat[58]))dd=3700+((val-tem_dat[57])*100)/(tem_dat[58]-tem_dat[57]);
    else if(val>=tem_dat[58]&&val<(tem_dat[59]))dd=3800+((val-tem_dat[58])*100)/(tem_dat[59]-tem_dat[58]);
    else if(val>=tem_dat[59]&&val<(tem_dat[60]))dd=3900+((val-tem_dat[59])*100)/(tem_dat[60]-tem_dat[59]);
	else if(val>=tem_dat[60]&&val<(tem_dat[61]))dd=4000+((val-tem_dat[60])*100)/(tem_dat[61]-tem_dat[60]);
    
    //----********41~50**变化量是9/2***********
    else if(val>=tem_dat[61]&&val<(tem_dat[62]))dd=4100+((val-tem_dat[61])*100)/(tem_dat[62]-tem_dat[61]);
    else if(val>=tem_dat[62]&&val<(tem_dat[63]))dd=4200+((val-tem_dat[62])*100)/(tem_dat[63]-tem_dat[62]);
    else if(val>=tem_dat[63]&&val<(tem_dat[64]))dd=4300+((val-tem_dat[63])*100)/(tem_dat[64]-tem_dat[63]);
    else if(val>=tem_dat[64]&&val<(tem_dat[65]))dd=4400+((val-tem_dat[64])*100)/(tem_dat[65]-tem_dat[64]);
    else if(val>=tem_dat[65]&&val<(tem_dat[66]))dd=4500+((val-tem_dat[65])*100)/(tem_dat[66]-tem_dat[65]);
    else if(val>=tem_dat[66]&&val<(tem_dat[67]))dd=4600+((val-tem_dat[66])*100)/(tem_dat[67]-tem_dat[66]);
    else if(val>=tem_dat[67]&&val<(tem_dat[68]))dd=4700+((val-tem_dat[67])*100)/(tem_dat[68]-tem_dat[67]);
    else if(val>=tem_dat[68]&&val<(tem_dat[69]))dd=4800+((val-tem_dat[68])*100)/(tem_dat[69]-tem_dat[68]);
    else if(val>=tem_dat[69]&&val<(tem_dat[70]))dd=4900+((val-tem_dat[69])*100)/(tem_dat[70]-tem_dat[69]);
	else if(val>=tem_dat[70]&&val<(tem_dat[71]))dd=5000+((val-tem_dat[70])*100)/(tem_dat[71]-tem_dat[70]);

    //----********51~60**变化量是6/2***********
    else if(val>=tem_dat[71]&&val<(tem_dat[72]))dd=5100+((val-tem_dat[71])*100)/(tem_dat[72]-tem_dat[71]);
    else if(val>=tem_dat[72]&&val<(tem_dat[73]))dd=5200+((val-tem_dat[72])*100)/(tem_dat[73]-tem_dat[72]);
    else if(val>=tem_dat[73]&&val<(tem_dat[74]))dd=5300+((val-tem_dat[73])*100)/(tem_dat[74]-tem_dat[73]);
    else if(val>=tem_dat[74]&&val<(tem_dat[75]))dd=5400+((val-tem_dat[74])*100)/(tem_dat[75]-tem_dat[74]);
    else if(val>=tem_dat[75]&&val<(tem_dat[76]))dd=5500+((val-tem_dat[75])*100)/(tem_dat[76]-tem_dat[75]);
    else if(val>=tem_dat[76]&&val<(tem_dat[77]))dd=5600+((val-tem_dat[76])*100)/(tem_dat[77]-tem_dat[76]);
    else if(val>=tem_dat[77]&&val<(tem_dat[78]))dd=5700+((val-tem_dat[77])*100)/(tem_dat[78]-tem_dat[77]);
    else if(val>=tem_dat[78]&&val<(tem_dat[79]))dd=5800+((val-tem_dat[78])*100)/(tem_dat[79]-tem_dat[78]);
    else if(val>=tem_dat[79]&&val<(tem_dat[80]))dd=5900+((val-tem_dat[79])*100)/(tem_dat[80]-tem_dat[79]);
	else if(val>=tem_dat[80]&&val<(tem_dat[81]))dd=6000+((val-tem_dat[80])*100)/(tem_dat[81]-tem_dat[80]);

   //-------********61~70**变化量是5/2***********
   	else if(val>=tem_dat[81]&&val<(tem_dat[82]))dd=6100+((val-tem_dat[81])*100)/(tem_dat[82]-tem_dat[81]);
    else if(val>=tem_dat[82]&&val<(tem_dat[83]))dd=6200+((val-tem_dat[82])*100)/(tem_dat[83]-tem_dat[82]);
    else if(val>=tem_dat[83]&&val<(tem_dat[84]))dd=6300+((val-tem_dat[83])*100)/(tem_dat[84]-tem_dat[83]);
    else if(val>=tem_dat[84]&&val<(tem_dat[85]))dd=6400+((val-tem_dat[84])*100)/(tem_dat[85]-tem_dat[84]);
    else if(val>=tem_dat[85]&&val<(tem_dat[86]))dd=6500+((val-tem_dat[85])*100)/(tem_dat[86]-tem_dat[85]);
    else if(val>=tem_dat[86]&&val<(tem_dat[87]))dd=6600+((val-tem_dat[86])*100)/(tem_dat[87]-tem_dat[86]);
    else if(val>=tem_dat[87]&&val<(tem_dat[88]))dd=6700+((val-tem_dat[87])*100)/(tem_dat[88]-tem_dat[87]);
    else if(val>=tem_dat[88]&&val<(tem_dat[89]))dd=6800+((val-tem_dat[88])*100)/(tem_dat[89]-tem_dat[88]);
    else if(val>=tem_dat[89]&&val<(tem_dat[90]))dd=6900+((val-tem_dat[89])*100)/(tem_dat[90]-tem_dat[89]);
	else if(val>=tem_dat[90]&&val<(tem_dat[91]))dd=7000+((val-tem_dat[90])*100)/(tem_dat[91]-tem_dat[90]);

   //------*******71~80**变化量是4/2***********
 	else if(val>=tem_dat[91]&&val<(tem_dat[92]))dd=7100+((val-tem_dat[91])*100)/(tem_dat[92]-tem_dat[91]);
    else if(val>=tem_dat[92]&&val<(tem_dat[93]))dd=7200+((val-tem_dat[92])*100)/(tem_dat[93]-tem_dat[92]);
    else if(val>=tem_dat[93]&&val<(tem_dat[94]))dd=7300+((val-tem_dat[93])*100)/(tem_dat[94]-tem_dat[93]);
    else if(val>=tem_dat[94]&&val<(tem_dat[95]))dd=7400+((val-tem_dat[94])*100)/(tem_dat[95]-tem_dat[94]);
    else if(val>=tem_dat[95]&&val<(tem_dat[96]))dd=7500+((val-tem_dat[95])*100)/(tem_dat[96]-tem_dat[95]);
    else if(val>=tem_dat[96]&&val<(tem_dat[97]))dd=7600+((val-tem_dat[96])*100)/(tem_dat[97]-tem_dat[96]);
    else if(val>=tem_dat[97]&&val<(tem_dat[98]))dd=7700+((val-tem_dat[97])*100)/(tem_dat[98]-tem_dat[97]);
    else if(val>=tem_dat[98]&&val<(tem_dat[99]))dd=7800+((val-tem_dat[98])*100)/(tem_dat[99]-tem_dat[98]);
    else if(val>=tem_dat[99]&&val<(tem_dat[100]))dd=7900+((val-tem_dat[99])*100)/(tem_dat[100]-tem_dat[99]);
	else if(val>=tem_dat[100]&&val<(tem_dat[101]))dd=8000+((val-tem_dat[100])*100)/(tem_dat[101]-tem_dat[100]);
   

    //*************81~90**变化量是3/2***********
	else if(val>=tem_dat[101]&&val<(tem_dat[102]))dd=8100+((val-tem_dat[101])*100)/(tem_dat[102]-tem_dat[101]);
    else if(val>=tem_dat[102]&&val<(tem_dat[103]))dd=8200+((val-tem_dat[102])*100)/(tem_dat[103]-tem_dat[102]);
    else if(val>=tem_dat[103]&&val<(tem_dat[104]))dd=8300+((val-tem_dat[103])*100)/(tem_dat[104]-tem_dat[103]);
    else if(val>=tem_dat[104]&&val<(tem_dat[105]))dd=8400+((val-tem_dat[104])*100)/(tem_dat[105]-tem_dat[104]);
    else if(val>=tem_dat[105]&&val<(tem_dat[106]))dd=8500+((val-tem_dat[105])*100)/(tem_dat[106]-tem_dat[105]);
    else if(val>=tem_dat[106]&&val<(tem_dat[107]))dd=8600+((val-tem_dat[106])*100)/(tem_dat[107]-tem_dat[106]);
    else if(val>=tem_dat[107]&&val<(tem_dat[108]))dd=8700+((val-tem_dat[107])*100)/(tem_dat[108]-tem_dat[107]);
    else if(val>=tem_dat[108]&&val<(tem_dat[109]))dd=8800+((val-tem_dat[108])*100)/(tem_dat[109]-tem_dat[108]);
    else if(val>=tem_dat[109]&&val<(tem_dat[110]))dd=8900+((val-tem_dat[109])*100)/(tem_dat[110]-tem_dat[109]);
	else if(val>=tem_dat[110]&&val<(tem_dat[111]))dd=9000+((val-tem_dat[110])*100)/(tem_dat[111]-tem_dat[110]);
    

	//*************91~100**变化量是3/2***********
	else if(val>=tem_dat[111]&&val<(tem_dat[112]))dd=9100+((val-tem_dat[111])*100)/(tem_dat[112]-tem_dat[111]);
    else if(val>=tem_dat[112]&&val<(tem_dat[113]))dd=9200+((val-tem_dat[112])*100)/(tem_dat[113]-tem_dat[112]);
    else if(val>=tem_dat[113]&&val<(tem_dat[114]))dd=9300+((val-tem_dat[113])*100)/(tem_dat[114]-tem_dat[113]);
    else if(val>=tem_dat[114]&&val<(tem_dat[115]))dd=9400+((val-tem_dat[114])*100)/(tem_dat[115]-tem_dat[114]);
    else if(val>=tem_dat[115]&&val<(tem_dat[116]))dd=9500+((val-tem_dat[115])*100)/(tem_dat[116]-tem_dat[115]);
    else if(val>=tem_dat[116]&&val<(tem_dat[117]))dd=9600+((val-tem_dat[116])*100)/(tem_dat[117]-tem_dat[116]);
    else if(val>=tem_dat[117]&&val<(tem_dat[118]))dd=9700+((val-tem_dat[117])*100)/(tem_dat[118]-tem_dat[117]);
    else if(val>=tem_dat[118]&&val<(tem_dat[119]))dd=9800+((val-tem_dat[118])*100)/(tem_dat[119]-tem_dat[118]);
    else if(val>=tem_dat[119]&&val<(tem_dat[120]))dd=9900+((val-tem_dat[119])*100)/(tem_dat[120]-tem_dat[119]);
	else if(val>=tem_dat[120]&&val<(tem_dat[121]))dd=10000+((val-tem_dat[120])*100)/(tem_dat[121]-tem_dat[120]);
    
	else if(val>=tem_dat[121])dd=20000;
    return dd;
}

int temp_change_cold(unsigned int val)
{
    int dd;
	//int ad_value_temp;
    //val=val-32768;
    //---------------------------------------------------------------------------------

	if(val<tem_dat_cold[0])dd=-10000;
	else if(val>=tem_dat_cold[0]&&val<(tem_dat_cold[1]))dd=((val-tem_dat_cold[0])*100)/(tem_dat_cold[1]-tem_dat_cold[0])-2000;
    else if(val>=tem_dat_cold[1]&&val<(tem_dat_cold[2]))dd=((val-tem_dat_cold[1])*100)/(tem_dat_cold[2]-tem_dat_cold[1])-1900;
    else if(val>=tem_dat_cold[2]&&val<(tem_dat_cold[3]))dd=((val-tem_dat_cold[2])*100)/(tem_dat_cold[3]-tem_dat_cold[2])-1800;
    else if(val>=tem_dat_cold[3]&&val<(tem_dat_cold[4]))dd=((val-tem_dat_cold[3])*100)/(tem_dat_cold[4]-tem_dat_cold[3])-1700;
    else if(val>=tem_dat_cold[4]&&val<(tem_dat_cold[5]))dd=((val-tem_dat_cold[4])*100)/(tem_dat_cold[5]-tem_dat_cold[4])-1600;
    else if(val>=tem_dat_cold[5]&&val<(tem_dat_cold[6]))dd=((val-tem_dat_cold[5])*100)/(tem_dat_cold[6]-tem_dat_cold[5])-1500;
    else if(val>=tem_dat_cold[6]&&val<(tem_dat_cold[7]))dd=((val-tem_dat_cold[6])*100)/(tem_dat_cold[7]-tem_dat_cold[6])-1400;
    else if(val>=tem_dat_cold[7]&&val<(tem_dat_cold[8]))dd=((val-tem_dat_cold[7])*100)/(tem_dat_cold[8]-tem_dat_cold[7])-1300;
    else if(val>=tem_dat_cold[8]&&val<(tem_dat_cold[9]))dd=((val-tem_dat_cold[8])*100)/(tem_dat_cold[9]-tem_dat_cold[8])-1200;
    else if(val>=tem_dat_cold[9]&&val<(tem_dat_cold[10]))dd=((val-tem_dat_cold[9])*100)/(tem_dat_cold[10]-tem_dat_cold[9])-1100;
	else if(val>=tem_dat_cold[10]&&val<(tem_dat_cold[11]))dd=((val-tem_dat_cold[10])*100)/(tem_dat_cold[11]-tem_dat_cold[10])-1000;
   
   	else if(val>=tem_dat_cold[11]&&val<(tem_dat_cold[12]))dd=((val-tem_dat_cold[11])*100)/(tem_dat_cold[12]-tem_dat_cold[11])-900;
    else if(val>=tem_dat_cold[12]&&val<(tem_dat_cold[13]))dd=((val-tem_dat_cold[12])*100)/(tem_dat_cold[13]-tem_dat_cold[12])-800;
    else if(val>=tem_dat_cold[13]&&val<(tem_dat_cold[14]))dd=((val-tem_dat_cold[13])*100)/(tem_dat_cold[14]-tem_dat_cold[13])-700;
    else if(val>=tem_dat_cold[14]&&val<(tem_dat_cold[15]))dd=((val-tem_dat_cold[14])*100)/(tem_dat_cold[15]-tem_dat_cold[14])-600;
    else if(val>=tem_dat_cold[15]&&val<(tem_dat_cold[16]))dd=((val-tem_dat_cold[15])*100)/(tem_dat_cold[16]-tem_dat_cold[15])-500;
    else if(val>=tem_dat_cold[16]&&val<(tem_dat_cold[17]))dd=((val-tem_dat_cold[16])*100)/(tem_dat_cold[17]-tem_dat_cold[16])-400;
    else if(val>=tem_dat_cold[17]&&val<(tem_dat_cold[18]))dd=((val-tem_dat_cold[17])*100)/(tem_dat_cold[18]-tem_dat_cold[17])-300;
    else if(val>=tem_dat_cold[18]&&val<(tem_dat_cold[19]))dd=((val-tem_dat_cold[18])*100)/(tem_dat_cold[19]-tem_dat_cold[18])-200;
    else if(val>=tem_dat_cold[19]&&val<(tem_dat_cold[20]))dd=((val-tem_dat_cold[19])*100)/(tem_dat_cold[20]-tem_dat_cold[19])-100;
	else if(val>=tem_dat_cold[20]&&val<(tem_dat_cold[21]))dd=0+((val-tem_dat_cold[20])*100)/(tem_dat_cold[21]-tem_dat_cold[20]);
	
	else if(val>=tem_dat_cold[21]&&val<(tem_dat_cold[22]))dd=100+((val-tem_dat_cold[21])*100)/(tem_dat_cold[22]-tem_dat_cold[21]);
    else if(val>=tem_dat_cold[22]&&val<(tem_dat_cold[23]))dd=200+((val-tem_dat_cold[22])*100)/(tem_dat_cold[23]-tem_dat_cold[22]);
    else if(val>=tem_dat_cold[23]&&val<(tem_dat_cold[24]))dd=300+((val-tem_dat_cold[23])*100)/(tem_dat_cold[24]-tem_dat_cold[23]);
    else if(val>=tem_dat_cold[24]&&val<(tem_dat_cold[25]))dd=400+((val-tem_dat_cold[24])*100)/(tem_dat_cold[25]-tem_dat_cold[24]);
    else if(val>=tem_dat_cold[25]&&val<(tem_dat_cold[26]))dd=500+((val-tem_dat_cold[25])*100)/(tem_dat_cold[26]-tem_dat_cold[25]);
    else if(val>=tem_dat_cold[26]&&val<(tem_dat_cold[27]))dd=600+((val-tem_dat_cold[26])*100)/(tem_dat_cold[27]-tem_dat_cold[26]);
    else if(val>=tem_dat_cold[27]&&val<(tem_dat_cold[28]))dd=700+((val-tem_dat_cold[27])*100)/(tem_dat_cold[28]-tem_dat_cold[27]);
    else if(val>=tem_dat_cold[28]&&val<(tem_dat_cold[29]))dd=800+((val-tem_dat_cold[28])*100)/(tem_dat_cold[29]-tem_dat_cold[28]);
    else if(val>=tem_dat_cold[29]&&val<(tem_dat_cold[30]))dd=900+((val-tem_dat_cold[29])*100)/(tem_dat_cold[30]-tem_dat_cold[29]);
	else if(val>=tem_dat_cold[30]&&val<(tem_dat_cold[31]))dd=1000+((val-tem_dat_cold[30])*100)/(tem_dat_cold[31]-tem_dat_cold[30]);

	else if(val>=tem_dat_cold[31]&&val<(tem_dat_cold[32]))dd=1100+((val-tem_dat_cold[31])*100)/(tem_dat_cold[32]-tem_dat_cold[31]);
    else if(val>=tem_dat_cold[32]&&val<(tem_dat_cold[33]))dd=1200+((val-tem_dat_cold[32])*100)/(tem_dat_cold[33]-tem_dat_cold[32]);
    else if(val>=tem_dat_cold[33]&&val<(tem_dat_cold[34]))dd=1300+((val-tem_dat_cold[33])*100)/(tem_dat_cold[34]-tem_dat_cold[33]);
    else if(val>=tem_dat_cold[34]&&val<(tem_dat_cold[35]))dd=1400+((val-tem_dat_cold[34])*100)/(tem_dat_cold[35]-tem_dat_cold[34]);
    else if(val>=tem_dat_cold[35]&&val<(tem_dat_cold[36]))dd=1500+((val-tem_dat_cold[35])*100)/(tem_dat_cold[36]-tem_dat_cold[35]);
    else if(val>=tem_dat_cold[36]&&val<(tem_dat_cold[37]))dd=1600+((val-tem_dat_cold[36])*100)/(tem_dat_cold[37]-tem_dat_cold[36]);
    else if(val>=tem_dat_cold[37]&&val<(tem_dat_cold[38]))dd=1700+((val-tem_dat_cold[37])*100)/(tem_dat_cold[38]-tem_dat_cold[37]);
    else if(val>=tem_dat_cold[38]&&val<(tem_dat_cold[39]))dd=1800+((val-tem_dat_cold[38])*100)/(tem_dat_cold[39]-tem_dat_cold[38]);
    else if(val>=tem_dat_cold[39]&&val<(tem_dat_cold[40]))dd=1900+((val-tem_dat_cold[39])*100)/(tem_dat_cold[40]-tem_dat_cold[39]);
	else if(val>=tem_dat_cold[40]&&val<(tem_dat_cold[41]))dd=2000+((val-tem_dat_cold[40])*100)/(tem_dat_cold[41]-tem_dat_cold[40]);

   //----********21~30**变化量是10/2***********
    else if(val>=tem_dat_cold[41]&&val<(tem_dat_cold[42]))dd=2100+((val-tem_dat_cold[41])*100)/(tem_dat_cold[42]-tem_dat_cold[41]);
    else if(val>=tem_dat_cold[42]&&val<(tem_dat_cold[43]))dd=2200+((val-tem_dat_cold[42])*100)/(tem_dat_cold[43]-tem_dat_cold[42]);
    else if(val>=tem_dat_cold[43]&&val<(tem_dat_cold[44]))dd=2300+((val-tem_dat_cold[43])*100)/(tem_dat_cold[44]-tem_dat_cold[43]);
    else if(val>=tem_dat_cold[44]&&val<(tem_dat_cold[45]))dd=2400+((val-tem_dat_cold[44])*100)/(tem_dat_cold[45]-tem_dat_cold[44]);
    else if(val>=tem_dat_cold[45]&&val<(tem_dat_cold[46]))dd=2500+((val-tem_dat_cold[45])*100)/(tem_dat_cold[46]-tem_dat_cold[45]);
    else if(val>=tem_dat_cold[46]&&val<(tem_dat_cold[47]))dd=2600+((val-tem_dat_cold[46])*100)/(tem_dat_cold[47]-tem_dat_cold[46]);
    else if(val>=tem_dat_cold[47]&&val<(tem_dat_cold[48]))dd=2700+((val-tem_dat_cold[47])*100)/(tem_dat_cold[48]-tem_dat_cold[47]);
    else if(val>=tem_dat_cold[48]&&val<(tem_dat_cold[49]))dd=2800+((val-tem_dat_cold[48])*100)/(tem_dat_cold[49]-tem_dat_cold[48]);
    else if(val>=tem_dat_cold[49]&&val<(tem_dat_cold[50]))dd=2900+((val-tem_dat_cold[49])*100)/(tem_dat_cold[50]-tem_dat_cold[49]);
	else if(val>=tem_dat_cold[50]&&val<(tem_dat_cold[51]))dd=3000+((val-tem_dat_cold[50])*100)/(tem_dat_cold[51]-tem_dat_cold[50]);
    
    //*************31~40**变化量是9/2***********
    else if(val>=tem_dat_cold[51]&&val<(tem_dat_cold[52]))dd=3100+((val-tem_dat_cold[51])*100)/(tem_dat_cold[52]-tem_dat_cold[51]);
    else if(val>=tem_dat_cold[52]&&val<(tem_dat_cold[53]))dd=3200+((val-tem_dat_cold[52])*100)/(tem_dat_cold[53]-tem_dat_cold[52]);
    else if(val>=tem_dat_cold[53]&&val<(tem_dat_cold[54]))dd=3300+((val-tem_dat_cold[53])*100)/(tem_dat_cold[54]-tem_dat_cold[53]);
    else if(val>=tem_dat_cold[54]&&val<(tem_dat_cold[55]))dd=3400+((val-tem_dat_cold[54])*100)/(tem_dat_cold[55]-tem_dat_cold[54]);
    else if(val>=tem_dat_cold[55]&&val<(tem_dat_cold[56]))dd=3500+((val-tem_dat_cold[55])*100)/(tem_dat_cold[56]-tem_dat_cold[55]);
    else if(val>=tem_dat_cold[56]&&val<(tem_dat_cold[57]))dd=3600+((val-tem_dat_cold[56])*100)/(tem_dat_cold[57]-tem_dat_cold[56]);
    else if(val>=tem_dat_cold[57]&&val<(tem_dat_cold[58]))dd=3700+((val-tem_dat_cold[57])*100)/(tem_dat_cold[58]-tem_dat_cold[57]);
    else if(val>=tem_dat_cold[58]&&val<(tem_dat_cold[59]))dd=3800+((val-tem_dat_cold[58])*100)/(tem_dat_cold[59]-tem_dat_cold[58]);
    else if(val>=tem_dat_cold[59]&&val<(tem_dat_cold[60]))dd=3900+((val-tem_dat_cold[59])*100)/(tem_dat_cold[60]-tem_dat_cold[59]);
	else if(val>=tem_dat_cold[60]&&val<(tem_dat_cold[61]))dd=4000+((val-tem_dat_cold[60])*100)/(tem_dat_cold[61]-tem_dat_cold[60]);
    
    //----********41~50**变化量是9/2***********
    else if(val>=tem_dat_cold[61]&&val<(tem_dat_cold[62]))dd=4100+((val-tem_dat_cold[61])*100)/(tem_dat_cold[62]-tem_dat_cold[61]);
    else if(val>=tem_dat_cold[62]&&val<(tem_dat_cold[63]))dd=4200+((val-tem_dat_cold[62])*100)/(tem_dat_cold[63]-tem_dat_cold[62]);
    else if(val>=tem_dat_cold[63]&&val<(tem_dat_cold[64]))dd=4300+((val-tem_dat_cold[63])*100)/(tem_dat_cold[64]-tem_dat_cold[63]);
    else if(val>=tem_dat_cold[64]&&val<(tem_dat_cold[65]))dd=4400+((val-tem_dat_cold[64])*100)/(tem_dat_cold[65]-tem_dat_cold[64]);
    else if(val>=tem_dat_cold[65]&&val<(tem_dat_cold[66]))dd=4500+((val-tem_dat_cold[65])*100)/(tem_dat_cold[66]-tem_dat_cold[65]);
    else if(val>=tem_dat_cold[66]&&val<(tem_dat_cold[67]))dd=4600+((val-tem_dat_cold[66])*100)/(tem_dat_cold[67]-tem_dat_cold[66]);
    else if(val>=tem_dat_cold[67]&&val<(tem_dat_cold[68]))dd=4700+((val-tem_dat_cold[67])*100)/(tem_dat_cold[68]-tem_dat_cold[67]);
    else if(val>=tem_dat_cold[68]&&val<(tem_dat_cold[69]))dd=4800+((val-tem_dat_cold[68])*100)/(tem_dat_cold[69]-tem_dat_cold[68]);
    else if(val>=tem_dat_cold[69]&&val<(tem_dat_cold[70]))dd=4900+((val-tem_dat_cold[69])*100)/(tem_dat_cold[70]-tem_dat_cold[69]);
	else if(val>=tem_dat_cold[70]&&val<(tem_dat_cold[71]))dd=5000+((val-tem_dat_cold[70])*100)/(tem_dat_cold[71]-tem_dat_cold[70]);

    //----********51~60**变化量是6/2***********
    else if(val>=tem_dat_cold[71]&&val<(tem_dat_cold[72]))dd=5100+((val-tem_dat_cold[71])*100)/(tem_dat_cold[72]-tem_dat_cold[71]);
    else if(val>=tem_dat_cold[72]&&val<(tem_dat_cold[73]))dd=5200+((val-tem_dat_cold[72])*100)/(tem_dat_cold[73]-tem_dat_cold[72]);
    else if(val>=tem_dat_cold[73]&&val<(tem_dat_cold[74]))dd=5300+((val-tem_dat_cold[73])*100)/(tem_dat_cold[74]-tem_dat_cold[73]);
    else if(val>=tem_dat_cold[74]&&val<(tem_dat_cold[75]))dd=5400+((val-tem_dat_cold[74])*100)/(tem_dat_cold[75]-tem_dat_cold[74]);
    else if(val>=tem_dat_cold[75]&&val<(tem_dat_cold[76]))dd=5500+((val-tem_dat_cold[75])*100)/(tem_dat_cold[76]-tem_dat_cold[75]);
    else if(val>=tem_dat_cold[76]&&val<(tem_dat_cold[77]))dd=5600+((val-tem_dat_cold[76])*100)/(tem_dat_cold[77]-tem_dat_cold[76]);
    else if(val>=tem_dat_cold[77]&&val<(tem_dat_cold[78]))dd=5700+((val-tem_dat_cold[77])*100)/(tem_dat_cold[78]-tem_dat_cold[77]);
    else if(val>=tem_dat_cold[78]&&val<(tem_dat_cold[79]))dd=5800+((val-tem_dat_cold[78])*100)/(tem_dat_cold[79]-tem_dat_cold[78]);
    else if(val>=tem_dat_cold[79]&&val<(tem_dat_cold[80]))dd=5900+((val-tem_dat_cold[79])*100)/(tem_dat_cold[80]-tem_dat_cold[79]);
	else if(val>=tem_dat_cold[80]&&val<(tem_dat_cold[81]))dd=6000+((val-tem_dat_cold[80])*100)/(tem_dat_cold[81]-tem_dat_cold[80]);

   //-------********61~70**变化量是5/2***********
   	else if(val>=tem_dat_cold[81]&&val<(tem_dat_cold[82]))dd=6100+((val-tem_dat_cold[81])*100)/(tem_dat_cold[82]-tem_dat_cold[81]);
    else if(val>=tem_dat_cold[82]&&val<(tem_dat_cold[83]))dd=6200+((val-tem_dat_cold[82])*100)/(tem_dat_cold[83]-tem_dat_cold[82]);
    else if(val>=tem_dat_cold[83]&&val<(tem_dat_cold[84]))dd=6300+((val-tem_dat_cold[83])*100)/(tem_dat_cold[84]-tem_dat_cold[83]);
    else if(val>=tem_dat_cold[84]&&val<(tem_dat_cold[85]))dd=6400+((val-tem_dat_cold[84])*100)/(tem_dat_cold[85]-tem_dat_cold[84]);
    else if(val>=tem_dat_cold[85]&&val<(tem_dat_cold[86]))dd=6500+((val-tem_dat_cold[85])*100)/(tem_dat_cold[86]-tem_dat_cold[85]);
    else if(val>=tem_dat_cold[86]&&val<(tem_dat_cold[87]))dd=6600+((val-tem_dat_cold[86])*100)/(tem_dat_cold[87]-tem_dat_cold[86]);
    else if(val>=tem_dat_cold[87]&&val<(tem_dat_cold[88]))dd=6700+((val-tem_dat_cold[87])*100)/(tem_dat_cold[88]-tem_dat_cold[87]);
    else if(val>=tem_dat_cold[88]&&val<(tem_dat_cold[89]))dd=6800+((val-tem_dat_cold[88])*100)/(tem_dat_cold[89]-tem_dat_cold[88]);
    else if(val>=tem_dat_cold[89]&&val<(tem_dat_cold[90]))dd=6900+((val-tem_dat_cold[89])*100)/(tem_dat_cold[90]-tem_dat_cold[89]);
	else if(val>=tem_dat_cold[90]&&val<(tem_dat_cold[91]))dd=7000+((val-tem_dat_cold[90])*100)/(tem_dat_cold[91]-tem_dat_cold[90]);

   //------*******71~80**变化量是4/2***********
 	else if(val>=tem_dat_cold[91]&&val<(tem_dat_cold[92]))dd=7100+((val-tem_dat_cold[91])*100)/(tem_dat_cold[92]-tem_dat_cold[91]);
    else if(val>=tem_dat_cold[92]&&val<(tem_dat_cold[93]))dd=7200+((val-tem_dat_cold[92])*100)/(tem_dat_cold[93]-tem_dat_cold[92]);
    else if(val>=tem_dat_cold[93]&&val<(tem_dat_cold[94]))dd=7300+((val-tem_dat_cold[93])*100)/(tem_dat_cold[94]-tem_dat_cold[93]);
    else if(val>=tem_dat_cold[94]&&val<(tem_dat_cold[95]))dd=7400+((val-tem_dat_cold[94])*100)/(tem_dat_cold[95]-tem_dat_cold[94]);
    else if(val>=tem_dat_cold[95]&&val<(tem_dat_cold[96]))dd=7500+((val-tem_dat_cold[95])*100)/(tem_dat_cold[96]-tem_dat_cold[95]);
    else if(val>=tem_dat_cold[96]&&val<(tem_dat_cold[97]))dd=7600+((val-tem_dat_cold[96])*100)/(tem_dat_cold[97]-tem_dat_cold[96]);
    else if(val>=tem_dat_cold[97]&&val<(tem_dat_cold[98]))dd=7700+((val-tem_dat_cold[97])*100)/(tem_dat_cold[98]-tem_dat_cold[97]);
    else if(val>=tem_dat_cold[98]&&val<(tem_dat_cold[99]))dd=7800+((val-tem_dat_cold[98])*100)/(tem_dat_cold[99]-tem_dat_cold[98]);
    else if(val>=tem_dat_cold[99]&&val<(tem_dat_cold[100]))dd=7900+((val-tem_dat_cold[99])*100)/(tem_dat_cold[100]-tem_dat_cold[99]);
	else if(val>=tem_dat_cold[100]&&val<(tem_dat_cold[101]))dd=8000+((val-tem_dat_cold[100])*100)/(tem_dat_cold[101]-tem_dat_cold[100]);
   

    //*************81~90**变化量是3/2***********
	else if(val>=tem_dat_cold[101]&&val<(tem_dat_cold[102]))dd=8100+((val-tem_dat_cold[101])*100)/(tem_dat_cold[102]-tem_dat_cold[101]);
    else if(val>=tem_dat_cold[102]&&val<(tem_dat_cold[103]))dd=8200+((val-tem_dat_cold[102])*100)/(tem_dat_cold[103]-tem_dat_cold[102]);
    else if(val>=tem_dat_cold[103]&&val<(tem_dat_cold[104]))dd=8300+((val-tem_dat_cold[103])*100)/(tem_dat_cold[104]-tem_dat_cold[103]);
    else if(val>=tem_dat_cold[104]&&val<(tem_dat_cold[105]))dd=8400+((val-tem_dat_cold[104])*100)/(tem_dat_cold[105]-tem_dat_cold[104]);
    else if(val>=tem_dat_cold[105]&&val<(tem_dat_cold[106]))dd=8500+((val-tem_dat_cold[105])*100)/(tem_dat_cold[106]-tem_dat_cold[105]);
    else if(val>=tem_dat_cold[106]&&val<(tem_dat_cold[107]))dd=8600+((val-tem_dat_cold[106])*100)/(tem_dat_cold[107]-tem_dat_cold[106]);
    else if(val>=tem_dat_cold[107]&&val<(tem_dat_cold[108]))dd=8700+((val-tem_dat_cold[107])*100)/(tem_dat_cold[108]-tem_dat_cold[107]);
    else if(val>=tem_dat_cold[108]&&val<(tem_dat_cold[109]))dd=8800+((val-tem_dat_cold[108])*100)/(tem_dat_cold[109]-tem_dat_cold[108]);
    else if(val>=tem_dat_cold[109]&&val<(tem_dat_cold[110]))dd=8900+((val-tem_dat_cold[109])*100)/(tem_dat_cold[110]-tem_dat_cold[109]);
	else if(val>=tem_dat_cold[110]&&val<(tem_dat_cold[111]))dd=9000+((val-tem_dat_cold[110])*100)/(tem_dat_cold[111]-tem_dat_cold[110]);
    

	//*************91~100**变化量是3/2***********
	else if(val>=tem_dat_cold[111]&&val<(tem_dat_cold[112]))dd=9100+((val-tem_dat_cold[111])*100)/(tem_dat_cold[112]-tem_dat_cold[111]);
    else if(val>=tem_dat_cold[112]&&val<(tem_dat_cold[113]))dd=9200+((val-tem_dat_cold[112])*100)/(tem_dat_cold[113]-tem_dat_cold[112]);
    else if(val>=tem_dat_cold[113]&&val<(tem_dat_cold[114]))dd=9300+((val-tem_dat_cold[113])*100)/(tem_dat_cold[114]-tem_dat_cold[113]);
    else if(val>=tem_dat_cold[114]&&val<(tem_dat_cold[115]))dd=9400+((val-tem_dat_cold[114])*100)/(tem_dat_cold[115]-tem_dat_cold[114]);
    else if(val>=tem_dat_cold[115]&&val<(tem_dat_cold[116]))dd=9500+((val-tem_dat_cold[115])*100)/(tem_dat_cold[116]-tem_dat_cold[115]);
    else if(val>=tem_dat_cold[116]&&val<(tem_dat_cold[117]))dd=9600+((val-tem_dat_cold[116])*100)/(tem_dat_cold[117]-tem_dat_cold[116]);
    else if(val>=tem_dat_cold[117]&&val<(tem_dat_cold[118]))dd=9700+((val-tem_dat_cold[117])*100)/(tem_dat_cold[118]-tem_dat_cold[117]);
    else if(val>=tem_dat_cold[118]&&val<(tem_dat_cold[119]))dd=9800+((val-tem_dat_cold[118])*100)/(tem_dat_cold[119]-tem_dat_cold[118]);
    else if(val>=tem_dat_cold[119]&&val<(tem_dat_cold[120]))dd=9900+((val-tem_dat_cold[119])*100)/(tem_dat_cold[120]-tem_dat_cold[119]);
	else if(val>=tem_dat_cold[120]&&val<(tem_dat_cold[121]))dd=10000+((val-tem_dat_cold[120])*100)/(tem_dat_cold[121]-tem_dat_cold[120]);
    
	else if(val>=tem_dat_cold[121])dd=20000;
    return dd;
}
int temp_change_hot(unsigned int val)
{
    int dd;
	//int ad_value_temp;
    //val=val-32768;
    //---------------------------------------------------------------------------------

	if(val<tem_dat_hot[0])dd=-10000;
	else if(val>=tem_dat_hot[0]&&val<(tem_dat_hot[1]))dd=((val-tem_dat_hot[0])*100)/(tem_dat_hot[1]-tem_dat_hot[0])-2000;
    else if(val>=tem_dat_hot[1]&&val<(tem_dat_hot[2]))dd=((val-tem_dat_hot[1])*100)/(tem_dat_hot[2]-tem_dat_hot[1])-1900;
    else if(val>=tem_dat_hot[2]&&val<(tem_dat_hot[3]))dd=((val-tem_dat_hot[2])*100)/(tem_dat_hot[3]-tem_dat_hot[2])-1800;
    else if(val>=tem_dat_hot[3]&&val<(tem_dat_hot[4]))dd=((val-tem_dat_hot[3])*100)/(tem_dat_hot[4]-tem_dat_hot[3])-1700;
    else if(val>=tem_dat_hot[4]&&val<(tem_dat_hot[5]))dd=((val-tem_dat_hot[4])*100)/(tem_dat_hot[5]-tem_dat_hot[4])-1600;
    else if(val>=tem_dat_hot[5]&&val<(tem_dat_hot[6]))dd=((val-tem_dat_hot[5])*100)/(tem_dat_hot[6]-tem_dat_hot[5])-1500;
    else if(val>=tem_dat_hot[6]&&val<(tem_dat_hot[7]))dd=((val-tem_dat_hot[6])*100)/(tem_dat_hot[7]-tem_dat_hot[6])-1400;
    else if(val>=tem_dat_hot[7]&&val<(tem_dat_hot[8]))dd=((val-tem_dat_hot[7])*100)/(tem_dat_hot[8]-tem_dat_hot[7])-1300;
    else if(val>=tem_dat_hot[8]&&val<(tem_dat_hot[9]))dd=((val-tem_dat_hot[8])*100)/(tem_dat_hot[9]-tem_dat_hot[8])-1200;
    else if(val>=tem_dat_hot[9]&&val<(tem_dat_hot[10]))dd=((val-tem_dat_hot[9])*100)/(tem_dat_hot[10]-tem_dat_hot[9])-1100;
	else if(val>=tem_dat_hot[10]&&val<(tem_dat_hot[11]))dd=((val-tem_dat_hot[10])*100)/(tem_dat_hot[11]-tem_dat_hot[10])-1000;
   
   	else if(val>=tem_dat_hot[11]&&val<(tem_dat_hot[12]))dd=((val-tem_dat_hot[11])*100)/(tem_dat_hot[12]-tem_dat_hot[11])-900;
    else if(val>=tem_dat_hot[12]&&val<(tem_dat_hot[13]))dd=((val-tem_dat_hot[12])*100)/(tem_dat_hot[13]-tem_dat_hot[12])-800;
    else if(val>=tem_dat_hot[13]&&val<(tem_dat_hot[14]))dd=((val-tem_dat_hot[13])*100)/(tem_dat_hot[14]-tem_dat_hot[13])-700;
    else if(val>=tem_dat_hot[14]&&val<(tem_dat_hot[15]))dd=((val-tem_dat_hot[14])*100)/(tem_dat_hot[15]-tem_dat_hot[14])-600;
    else if(val>=tem_dat_hot[15]&&val<(tem_dat_hot[16]))dd=((val-tem_dat_hot[15])*100)/(tem_dat_hot[16]-tem_dat_hot[15])-500;
    else if(val>=tem_dat_hot[16]&&val<(tem_dat_hot[17]))dd=((val-tem_dat_hot[16])*100)/(tem_dat_hot[17]-tem_dat_hot[16])-400;
    else if(val>=tem_dat_hot[17]&&val<(tem_dat_hot[18]))dd=((val-tem_dat_hot[17])*100)/(tem_dat_hot[18]-tem_dat_hot[17])-300;
    else if(val>=tem_dat_hot[18]&&val<(tem_dat_hot[19]))dd=((val-tem_dat_hot[18])*100)/(tem_dat_hot[19]-tem_dat_hot[18])-200;
    else if(val>=tem_dat_hot[19]&&val<(tem_dat_hot[20]))dd=((val-tem_dat_hot[19])*100)/(tem_dat_hot[20]-tem_dat_hot[19])-100;
	else if(val>=tem_dat_hot[20]&&val<(tem_dat_hot[21]))dd=0+((val-tem_dat_hot[20])*100)/(tem_dat_hot[21]-tem_dat_hot[20]);
	
	else if(val>=tem_dat_hot[21]&&val<(tem_dat_hot[22]))dd=100+((val-tem_dat_hot[21])*100)/(tem_dat_hot[22]-tem_dat_hot[21]);
    else if(val>=tem_dat_hot[22]&&val<(tem_dat_hot[23]))dd=200+((val-tem_dat_hot[22])*100)/(tem_dat_hot[23]-tem_dat_hot[22]);
    else if(val>=tem_dat_hot[23]&&val<(tem_dat_hot[24]))dd=300+((val-tem_dat_hot[23])*100)/(tem_dat_hot[24]-tem_dat_hot[23]);
    else if(val>=tem_dat_hot[24]&&val<(tem_dat_hot[25]))dd=400+((val-tem_dat_hot[24])*100)/(tem_dat_hot[25]-tem_dat_hot[24]);
    else if(val>=tem_dat_hot[25]&&val<(tem_dat_hot[26]))dd=500+((val-tem_dat_hot[25])*100)/(tem_dat_hot[26]-tem_dat_hot[25]);
    else if(val>=tem_dat_hot[26]&&val<(tem_dat_hot[27]))dd=600+((val-tem_dat_hot[26])*100)/(tem_dat_hot[27]-tem_dat_hot[26]);
    else if(val>=tem_dat_hot[27]&&val<(tem_dat_hot[28]))dd=700+((val-tem_dat_hot[27])*100)/(tem_dat_hot[28]-tem_dat_hot[27]);
    else if(val>=tem_dat_hot[28]&&val<(tem_dat_hot[29]))dd=800+((val-tem_dat_hot[28])*100)/(tem_dat_hot[29]-tem_dat_hot[28]);
    else if(val>=tem_dat_hot[29]&&val<(tem_dat_hot[30]))dd=900+((val-tem_dat_hot[29])*100)/(tem_dat_hot[30]-tem_dat_hot[29]);
	else if(val>=tem_dat_hot[30]&&val<(tem_dat_hot[31]))dd=1000+((val-tem_dat_hot[30])*100)/(tem_dat_hot[31]-tem_dat_hot[30]);

	else if(val>=tem_dat_hot[31]&&val<(tem_dat_hot[32]))dd=1100+((val-tem_dat_hot[31])*100)/(tem_dat_hot[32]-tem_dat_hot[31]);
    else if(val>=tem_dat_hot[32]&&val<(tem_dat_hot[33]))dd=1200+((val-tem_dat_hot[32])*100)/(tem_dat_hot[33]-tem_dat_hot[32]);
    else if(val>=tem_dat_hot[33]&&val<(tem_dat_hot[34]))dd=1300+((val-tem_dat_hot[33])*100)/(tem_dat_hot[34]-tem_dat_hot[33]);
    else if(val>=tem_dat_hot[34]&&val<(tem_dat_hot[35]))dd=1400+((val-tem_dat_hot[34])*100)/(tem_dat_hot[35]-tem_dat_hot[34]);
    else if(val>=tem_dat_hot[35]&&val<(tem_dat_hot[36]))dd=1500+((val-tem_dat_hot[35])*100)/(tem_dat_hot[36]-tem_dat_hot[35]);
    else if(val>=tem_dat_hot[36]&&val<(tem_dat_hot[37]))dd=1600+((val-tem_dat_hot[36])*100)/(tem_dat_hot[37]-tem_dat_hot[36]);
    else if(val>=tem_dat_hot[37]&&val<(tem_dat_hot[38]))dd=1700+((val-tem_dat_hot[37])*100)/(tem_dat_hot[38]-tem_dat_hot[37]);
    else if(val>=tem_dat_hot[38]&&val<(tem_dat_hot[39]))dd=1800+((val-tem_dat_hot[38])*100)/(tem_dat_hot[39]-tem_dat_hot[38]);
    else if(val>=tem_dat_hot[39]&&val<(tem_dat_hot[40]))dd=1900+((val-tem_dat_hot[39])*100)/(tem_dat_hot[40]-tem_dat_hot[39]);
	else if(val>=tem_dat_hot[40]&&val<(tem_dat_hot[41]))dd=2000+((val-tem_dat_hot[40])*100)/(tem_dat_hot[41]-tem_dat_hot[40]);

   //----********21~30**变化量是10/2***********
    else if(val>=tem_dat_hot[41]&&val<(tem_dat_hot[42]))dd=2100+((val-tem_dat_hot[41])*100)/(tem_dat_hot[42]-tem_dat_hot[41]);
    else if(val>=tem_dat_hot[42]&&val<(tem_dat_hot[43]))dd=2200+((val-tem_dat_hot[42])*100)/(tem_dat_hot[43]-tem_dat_hot[42]);
    else if(val>=tem_dat_hot[43]&&val<(tem_dat_hot[44]))dd=2300+((val-tem_dat_hot[43])*100)/(tem_dat_hot[44]-tem_dat_hot[43]);
    else if(val>=tem_dat_hot[44]&&val<(tem_dat_hot[45]))dd=2400+((val-tem_dat_hot[44])*100)/(tem_dat_hot[45]-tem_dat_hot[44]);
    else if(val>=tem_dat_hot[45]&&val<(tem_dat_hot[46]))dd=2500+((val-tem_dat_hot[45])*100)/(tem_dat_hot[46]-tem_dat_hot[45]);
    else if(val>=tem_dat_hot[46]&&val<(tem_dat_hot[47]))dd=2600+((val-tem_dat_hot[46])*100)/(tem_dat_hot[47]-tem_dat_hot[46]);
    else if(val>=tem_dat_hot[47]&&val<(tem_dat_hot[48]))dd=2700+((val-tem_dat_hot[47])*100)/(tem_dat_hot[48]-tem_dat_hot[47]);
    else if(val>=tem_dat_hot[48]&&val<(tem_dat_hot[49]))dd=2800+((val-tem_dat_hot[48])*100)/(tem_dat_hot[49]-tem_dat_hot[48]);
    else if(val>=tem_dat_hot[49]&&val<(tem_dat_hot[50]))dd=2900+((val-tem_dat_hot[49])*100)/(tem_dat_hot[50]-tem_dat_hot[49]);
	else if(val>=tem_dat_hot[50]&&val<(tem_dat_hot[51]))dd=3000+((val-tem_dat_hot[50])*100)/(tem_dat_hot[51]-tem_dat_hot[50]);
    
    //*************31~40**变化量是9/2***********
    else if(val>=tem_dat_hot[51]&&val<(tem_dat_hot[52]))dd=3100+((val-tem_dat_hot[51])*100)/(tem_dat_hot[52]-tem_dat_hot[51]);
    else if(val>=tem_dat_hot[52]&&val<(tem_dat_hot[53]))dd=3200+((val-tem_dat_hot[52])*100)/(tem_dat_hot[53]-tem_dat_hot[52]);
    else if(val>=tem_dat_hot[53]&&val<(tem_dat_hot[54]))dd=3300+((val-tem_dat_hot[53])*100)/(tem_dat_hot[54]-tem_dat_hot[53]);
    else if(val>=tem_dat_hot[54]&&val<(tem_dat_hot[55]))dd=3400+((val-tem_dat_hot[54])*100)/(tem_dat_hot[55]-tem_dat_hot[54]);
    else if(val>=tem_dat_hot[55]&&val<(tem_dat_hot[56]))dd=3500+((val-tem_dat_hot[55])*100)/(tem_dat_hot[56]-tem_dat_hot[55]);
    else if(val>=tem_dat_hot[56]&&val<(tem_dat_hot[57]))dd=3600+((val-tem_dat_hot[56])*100)/(tem_dat_hot[57]-tem_dat_hot[56]);
    else if(val>=tem_dat_hot[57]&&val<(tem_dat_hot[58]))dd=3700+((val-tem_dat_hot[57])*100)/(tem_dat_hot[58]-tem_dat_hot[57]);
    else if(val>=tem_dat_hot[58]&&val<(tem_dat_hot[59]))dd=3800+((val-tem_dat_hot[58])*100)/(tem_dat_hot[59]-tem_dat_hot[58]);
    else if(val>=tem_dat_hot[59]&&val<(tem_dat_hot[60]))dd=3900+((val-tem_dat_hot[59])*100)/(tem_dat_hot[60]-tem_dat_hot[59]);
	else if(val>=tem_dat_hot[60]&&val<(tem_dat_hot[61]))dd=4000+((val-tem_dat_hot[60])*100)/(tem_dat_hot[61]-tem_dat_hot[60]);
    
    //----********41~50**变化量是9/2***********
    else if(val>=tem_dat_hot[61]&&val<(tem_dat_hot[62]))dd=4100+((val-tem_dat_hot[61])*100)/(tem_dat_hot[62]-tem_dat_hot[61]);
    else if(val>=tem_dat_hot[62]&&val<(tem_dat_hot[63]))dd=4200+((val-tem_dat_hot[62])*100)/(tem_dat_hot[63]-tem_dat_hot[62]);
    else if(val>=tem_dat_hot[63]&&val<(tem_dat_hot[64]))dd=4300+((val-tem_dat_hot[63])*100)/(tem_dat_hot[64]-tem_dat_hot[63]);
    else if(val>=tem_dat_hot[64]&&val<(tem_dat_hot[65]))dd=4400+((val-tem_dat_hot[64])*100)/(tem_dat_hot[65]-tem_dat_hot[64]);
    else if(val>=tem_dat_hot[65]&&val<(tem_dat_hot[66]))dd=4500+((val-tem_dat_hot[65])*100)/(tem_dat_hot[66]-tem_dat_hot[65]);
    else if(val>=tem_dat_hot[66]&&val<(tem_dat_hot[67]))dd=4600+((val-tem_dat_hot[66])*100)/(tem_dat_hot[67]-tem_dat_hot[66]);
    else if(val>=tem_dat_hot[67]&&val<(tem_dat_hot[68]))dd=4700+((val-tem_dat_hot[67])*100)/(tem_dat_hot[68]-tem_dat_hot[67]);
    else if(val>=tem_dat_hot[68]&&val<(tem_dat_hot[69]))dd=4800+((val-tem_dat_hot[68])*100)/(tem_dat_hot[69]-tem_dat_hot[68]);
    else if(val>=tem_dat_hot[69]&&val<(tem_dat_hot[70]))dd=4900+((val-tem_dat_hot[69])*100)/(tem_dat_hot[70]-tem_dat_hot[69]);
	else if(val>=tem_dat_hot[70]&&val<(tem_dat_hot[71]))dd=5000+((val-tem_dat_hot[70])*100)/(tem_dat_hot[71]-tem_dat_hot[70]);

    //----********51~60**变化量是6/2***********
    else if(val>=tem_dat_hot[71]&&val<(tem_dat_hot[72]))dd=5100+((val-tem_dat_hot[71])*100)/(tem_dat_hot[72]-tem_dat_hot[71]);
    else if(val>=tem_dat_hot[72]&&val<(tem_dat_hot[73]))dd=5200+((val-tem_dat_hot[72])*100)/(tem_dat_hot[73]-tem_dat_hot[72]);
    else if(val>=tem_dat_hot[73]&&val<(tem_dat_hot[74]))dd=5300+((val-tem_dat_hot[73])*100)/(tem_dat_hot[74]-tem_dat_hot[73]);
    else if(val>=tem_dat_hot[74]&&val<(tem_dat_hot[75]))dd=5400+((val-tem_dat_hot[74])*100)/(tem_dat_hot[75]-tem_dat_hot[74]);
    else if(val>=tem_dat_hot[75]&&val<(tem_dat_hot[76]))dd=5500+((val-tem_dat_hot[75])*100)/(tem_dat_hot[76]-tem_dat_hot[75]);
    else if(val>=tem_dat_hot[76]&&val<(tem_dat_hot[77]))dd=5600+((val-tem_dat_hot[76])*100)/(tem_dat_hot[77]-tem_dat_hot[76]);
    else if(val>=tem_dat_hot[77]&&val<(tem_dat_hot[78]))dd=5700+((val-tem_dat_hot[77])*100)/(tem_dat_hot[78]-tem_dat_hot[77]);
    else if(val>=tem_dat_hot[78]&&val<(tem_dat_hot[79]))dd=5800+((val-tem_dat_hot[78])*100)/(tem_dat_hot[79]-tem_dat_hot[78]);
    else if(val>=tem_dat_hot[79]&&val<(tem_dat_hot[80]))dd=5900+((val-tem_dat_hot[79])*100)/(tem_dat_hot[80]-tem_dat_hot[79]);
	else if(val>=tem_dat_hot[80]&&val<(tem_dat_hot[81]))dd=6000+((val-tem_dat_hot[80])*100)/(tem_dat_hot[81]-tem_dat_hot[80]);

   //-------********61~70**变化量是5/2***********
   	else if(val>=tem_dat_hot[81]&&val<(tem_dat_hot[82]))dd=6100+((val-tem_dat_hot[81])*100)/(tem_dat_hot[82]-tem_dat_hot[81]);
    else if(val>=tem_dat_hot[82]&&val<(tem_dat_hot[83]))dd=6200+((val-tem_dat_hot[82])*100)/(tem_dat_hot[83]-tem_dat_hot[82]);
    else if(val>=tem_dat_hot[83]&&val<(tem_dat_hot[84]))dd=6300+((val-tem_dat_hot[83])*100)/(tem_dat_hot[84]-tem_dat_hot[83]);
    else if(val>=tem_dat_hot[84]&&val<(tem_dat_hot[85]))dd=6400+((val-tem_dat_hot[84])*100)/(tem_dat_hot[85]-tem_dat_hot[84]);
    else if(val>=tem_dat_hot[85]&&val<(tem_dat_hot[86]))dd=6500+((val-tem_dat_hot[85])*100)/(tem_dat_hot[86]-tem_dat_hot[85]);
    else if(val>=tem_dat_hot[86]&&val<(tem_dat_hot[87]))dd=6600+((val-tem_dat_hot[86])*100)/(tem_dat_hot[87]-tem_dat_hot[86]);
    else if(val>=tem_dat_hot[87]&&val<(tem_dat_hot[88]))dd=6700+((val-tem_dat_hot[87])*100)/(tem_dat_hot[88]-tem_dat_hot[87]);
    else if(val>=tem_dat_hot[88]&&val<(tem_dat_hot[89]))dd=6800+((val-tem_dat_hot[88])*100)/(tem_dat_hot[89]-tem_dat_hot[88]);
    else if(val>=tem_dat_hot[89]&&val<(tem_dat_hot[90]))dd=6900+((val-tem_dat_hot[89])*100)/(tem_dat_hot[90]-tem_dat_hot[89]);
	else if(val>=tem_dat_hot[90]&&val<(tem_dat_hot[91]))dd=7000+((val-tem_dat_hot[90])*100)/(tem_dat_hot[91]-tem_dat_hot[90]);

   //------*******71~80**变化量是4/2***********
 	else if(val>=tem_dat_hot[91]&&val<(tem_dat_hot[92]))dd=7100+((val-tem_dat_hot[91])*100)/(tem_dat_hot[92]-tem_dat_hot[91]);
    else if(val>=tem_dat_hot[92]&&val<(tem_dat_hot[93]))dd=7200+((val-tem_dat_hot[92])*100)/(tem_dat_hot[93]-tem_dat_hot[92]);
    else if(val>=tem_dat_hot[93]&&val<(tem_dat_hot[94]))dd=7300+((val-tem_dat_hot[93])*100)/(tem_dat_hot[94]-tem_dat_hot[93]);
    else if(val>=tem_dat_hot[94]&&val<(tem_dat_hot[95]))dd=7400+((val-tem_dat_hot[94])*100)/(tem_dat_hot[95]-tem_dat_hot[94]);
    else if(val>=tem_dat_hot[95]&&val<(tem_dat_hot[96]))dd=7500+((val-tem_dat_hot[95])*100)/(tem_dat_hot[96]-tem_dat_hot[95]);
    else if(val>=tem_dat_hot[96]&&val<(tem_dat_hot[97]))dd=7600+((val-tem_dat_hot[96])*100)/(tem_dat_hot[97]-tem_dat_hot[96]);
    else if(val>=tem_dat_hot[97]&&val<(tem_dat_hot[98]))dd=7700+((val-tem_dat_hot[97])*100)/(tem_dat_hot[98]-tem_dat_hot[97]);
    else if(val>=tem_dat_hot[98]&&val<(tem_dat_hot[99]))dd=7800+((val-tem_dat_hot[98])*100)/(tem_dat_hot[99]-tem_dat_hot[98]);
    else if(val>=tem_dat_hot[99]&&val<(tem_dat_hot[100]))dd=7900+((val-tem_dat_hot[99])*100)/(tem_dat_hot[100]-tem_dat_hot[99]);
	else if(val>=tem_dat_hot[100]&&val<(tem_dat_hot[101]))dd=8000+((val-tem_dat_hot[100])*100)/(tem_dat_hot[101]-tem_dat_hot[100]);
   

    //*************81~90**变化量是3/2***********
	else if(val>=tem_dat_hot[101]&&val<(tem_dat_hot[102]))dd=8100+((val-tem_dat_hot[101])*100)/(tem_dat_hot[102]-tem_dat_hot[101]);
    else if(val>=tem_dat_hot[102]&&val<(tem_dat_hot[103]))dd=8200+((val-tem_dat_hot[102])*100)/(tem_dat_hot[103]-tem_dat_hot[102]);
    else if(val>=tem_dat_hot[103]&&val<(tem_dat_hot[104]))dd=8300+((val-tem_dat_hot[103])*100)/(tem_dat_hot[104]-tem_dat_hot[103]);
    else if(val>=tem_dat_hot[104]&&val<(tem_dat_hot[105]))dd=8400+((val-tem_dat_hot[104])*100)/(tem_dat_hot[105]-tem_dat_hot[104]);
    else if(val>=tem_dat_hot[105]&&val<(tem_dat_hot[106]))dd=8500+((val-tem_dat_hot[105])*100)/(tem_dat_hot[106]-tem_dat_hot[105]);
    else if(val>=tem_dat_hot[106]&&val<(tem_dat_hot[107]))dd=8600+((val-tem_dat_hot[106])*100)/(tem_dat_hot[107]-tem_dat_hot[106]);
    else if(val>=tem_dat_hot[107]&&val<(tem_dat_hot[108]))dd=8700+((val-tem_dat_hot[107])*100)/(tem_dat_hot[108]-tem_dat_hot[107]);
    else if(val>=tem_dat_hot[108]&&val<(tem_dat_hot[109]))dd=8800+((val-tem_dat_hot[108])*100)/(tem_dat_hot[109]-tem_dat_hot[108]);
    else if(val>=tem_dat_hot[109]&&val<(tem_dat_hot[110]))dd=8900+((val-tem_dat_hot[109])*100)/(tem_dat_hot[110]-tem_dat_hot[109]);
	else if(val>=tem_dat_hot[110]&&val<(tem_dat_hot[111]))dd=9000+((val-tem_dat_hot[110])*100)/(tem_dat_hot[111]-tem_dat_hot[110]);
    

	//*************91~100**变化量是3/2***********
	else if(val>=tem_dat_hot[111]&&val<(tem_dat_hot[112]))dd=9100+((val-tem_dat_hot[111])*100)/(tem_dat_hot[112]-tem_dat_hot[111]);
    else if(val>=tem_dat_hot[112]&&val<(tem_dat_hot[113]))dd=9200+((val-tem_dat_hot[112])*100)/(tem_dat_hot[113]-tem_dat_hot[112]);
    else if(val>=tem_dat_hot[113]&&val<(tem_dat_hot[114]))dd=9300+((val-tem_dat_hot[113])*100)/(tem_dat_hot[114]-tem_dat_hot[113]);
    else if(val>=tem_dat_hot[114]&&val<(tem_dat_hot[115]))dd=9400+((val-tem_dat_hot[114])*100)/(tem_dat_hot[115]-tem_dat_hot[114]);
    else if(val>=tem_dat_hot[115]&&val<(tem_dat_hot[116]))dd=9500+((val-tem_dat_hot[115])*100)/(tem_dat_hot[116]-tem_dat_hot[115]);
    else if(val>=tem_dat_hot[116]&&val<(tem_dat_hot[117]))dd=9600+((val-tem_dat_hot[116])*100)/(tem_dat_hot[117]-tem_dat_hot[116]);
    else if(val>=tem_dat_hot[117]&&val<(tem_dat_hot[118]))dd=9700+((val-tem_dat_hot[117])*100)/(tem_dat_hot[118]-tem_dat_hot[117]);
    else if(val>=tem_dat_hot[118]&&val<(tem_dat_hot[119]))dd=9800+((val-tem_dat_hot[118])*100)/(tem_dat_hot[119]-tem_dat_hot[118]);
    else if(val>=tem_dat_hot[119]&&val<(tem_dat_hot[120]))dd=9900+((val-tem_dat_hot[119])*100)/(tem_dat_hot[120]-tem_dat_hot[119]);
	else if(val>=tem_dat_hot[120]&&val<(tem_dat_hot[121]))dd=10000+((val-tem_dat_hot[120])*100)/(tem_dat_hot[121]-tem_dat_hot[120]);
    
	else if(val>=tem_dat_hot[121])dd=20000;
    return dd;
}