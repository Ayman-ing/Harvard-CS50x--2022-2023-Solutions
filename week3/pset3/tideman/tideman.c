#include <cs50.h>
#include <stdio.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            //preferences[i][j]= 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        int j=0;
        bool test=true;
        while(candidates[i][j] != '\0' && test==true)
        {
            if (candidates[i][j]==name[j])
            {
                j=j+1;

            }
            else
            {
                test=false;
            }
        }

        if (test==true)
        {
            ranks[rank]=i;
            return true;
        }


    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i=0;i<candidate_count-1;i++)
    {
        for (int j=i;j < candidate_count-1;j++)
        {
            preferences[ranks[i]][ranks[j+1]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int k=0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count ; j++)
        {
            if (preferences[i][j] > preferences[j][i])
                {
                    pairs[k].winner=i;
                    pairs[k].loser=j;
                    k++;
                    pair_count++;

                }
        }
    }
      // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int i,j,cp=-1;
    while(cp!=0)
    {
        cp=0;
        for (i=0;i<pair_count;i++)
        {
            if(preferences[pairs[i].winner][pairs[i].loser] <  preferences[pairs[i+1].winner][pairs[i+1].loser])
            {
                pair temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
                cp++;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
bool cycle(int w,int l);
void lock_pairs(void)
{
    // TODO
    //int cpl=0;
    //int cpc=0;
    //int i=0;
    locked[pairs[0].winner][pairs[0].loser]=true;
    int winner;
    int loser;
    for(int i=0; i < pair_count;i++)
    {
        winner=pairs[i].winner;
        loser=pairs[i].loser;
        if(!cycle(winner,loser))
            locked[pairs[i].winner][pairs[i].loser]=true;

    }

    return;
}
bool cycle(int w,int l)
{

    if (locked[l][w]==true)
        return(true);
    else
    {
        for (int i=0;i < candidate_count ;i++)
        {
            if(locked[l][i]==true && cycle(w,i))
            {
                return(true);
            }

        }
    }
    return(false);
}

// Print the winner of the election
int recherche(int i);
void print_winner(void)
{
    // TODO
    int max=0;
    int k=0;
    for(int i=0;i < candidate_count -1 ; i++)
    {

        {
            int s=0;
            for(int j=0; j < candidate_count ;j++)
            {
                if(locked[j][i]==false)
                {
                    s++;
                }
            }
            if (s>max)
            {
                max=s;
                k=i;

            }
            if (s==max)
            {
               if(recherche(i) < recherche(k))
               {
                max=s;
                k=i;
               }
            }
        }
    }
    printf("%s\n",candidates[k]);

    return;
}
int recherche(int i)
{
    for(int j=0;j<pair_count -1 ;j++)
    {
        if (i==pairs[j].winner)
            return(j);
    }
    return(10);
}

//int min=0;
  //  int k=0;
    //for(int i=0;i < pair_count-1 ; i++)
    //{
      //  int j=pairs[i].winner;
        //{
          //  int s=0;
            //for(int l=0; l < candidate_count ;l++)
            //{
              //  if(locked[j][l]==false)
                //{
                  //  s++;
                //}
        //    }
          //  if (s<min)
            //{
              //  min=s;
                //k=j;
            //}
        //}
    //}