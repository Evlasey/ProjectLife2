#include<iostream>
#include<omp.h>
#include <fstream> // ������ � �������


using namespace std;

bool** arr;//�������� ������	
bool** new_arr;//����� ��� ������� ����. ����
int n, m, t;//n,m - ������ �������, t - ���������� ����������� �����
int num_threads = 4;

void copy_array()//����������� ������� new_arr � arr � ������ �����
{
	//����������� (��������) �����
	#pragma omp parallel for collapse(2)
	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= m; j++)
		{
			arr[i][j] = new_arr[i][j];
		}
	}

	//������ � ����� ������� (��� �����)
	#pragma omp parallel for
	for(int i = 1; i <= n; i++)
	{
		arr[i][0] = arr[i][m];
		arr[i][m+1] = arr[i][1];
	}

	//������� � ������ ������� (��� �����)
	#pragma omp parallel for
	for(int i = 1; i <= m; i++)
	{
		arr[0][i] = arr[n][i];
		arr[n+1][i] = arr[1][i];
	}

	//����
	arr[0][0] = arr[n][m];
	arr[0][m+1] = arr[n][1];
	arr[n+1][0] = arr[1][m];
	arr[n+1][m+1] = arr[1][1];
}

void read_data()//���������� ������ �� ����� � ����� ������ �������
{
	int k, x, y;// k - ����������� �����
	cin >> n >> m >> num_threads >> t >> k;
	
	//�������� �������� � ��������� ��������
	arr = new bool*[n+2];
	new_arr = new bool*[n+2];
	for(int i = 0; i < n+2; i++)
	{
		arr[i] = new bool[m+2];
		new_arr[i] = new bool[m+2];
		for(int j = 0; j < m+2; j++)
		{
			arr[i][j] = false;
			new_arr[i][j] = false;
		}
	}

	//���������� �����
	for(int i = 0; i < k; i++)
	{
		cin >> x >> y;
		new_arr[x][y] = true;
	}

	//��������� �����������
	copy_array();
}

void write_array() //��� ��������
{
	system("cls");
	for(int i = 0; i < n+2; i++)
	{
		for(int j = 0; j < m+2; j++)
		{
			if(arr[i][j])
				cout << "0 ";
			else
				cout << ". ";
		}
		cout << endl;
	}
}

int main()
{
	freopen("cells_data.txt","r",stdin);
	ofstream fout("alg_1_runtime.txt", ios_base::app);
	read_data();
	omp_set_dynamic(0);
	omp_set_num_threads( num_threads );
	double start_time = omp_get_wtime( );
	for(int counter = 0; counter < t; counter++)
	{	
		#pragma omp parallel for collapse(2)
		for(int i = 1; i <= n; i++)
		{
			for(int j = 1; j <= m; j++)
			{
				int k = 0;
				if(arr[i][j+1]) k++;
				if(arr[i+1][j]) k++;
				if(arr[i][j-1]) k++;
				if(arr[i-1][j]) k++;
				if(arr[i+1][j+1]) k++;
				if(arr[i-1][j+1]) k++;
				if(arr[i+1][j-1]) k++;
				if(arr[i-1][j-1]) k++;
				
				if(arr[i][j] && k != 3 && k != 2)//�����������
					new_arr[i][j] = false;
				if(!arr[i][j] && k == 3)//��������
					new_arr[i][j] = true;
				//� ������ ������� ������ �� ������
			}
		}
		//�������� ��� ���� �������� � �������� ������
		copy_array();
	}
	fout << " " << omp_get_wtime( ) - start_time;
	fout.close();
	return 0;
}