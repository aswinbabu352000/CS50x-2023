#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
} pair;

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
int strength(int n);
void lock_pairs(void);
void print_winner(void);
bool cycle(int winner, int loser);
bool won_check(int i);
bool lost_check(int i);

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

        /////////////////
        for (int l = 0; l < candidate_count; l++)
        {
            printf("%i ", ranks[l]);
        }

        printf("\n");
        //////////////////

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
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO

    // printing preferences
    printf("\n");

    for (int k = 0; k < candidate_count; k++)
    {
        for (int l = 0; l < candidate_count; l++)
        {
            printf("%i ", preferences[k][l]);
        }

        printf("\n");
    }

    //////////////////////////////////////////////////////

    int k = 0;
    for (int i = 0; i < candidate_count; i++)
    {

        for (int j = i + 1; j < candidate_count; j++)
        {

            if (preferences[i][j] > preferences[j][i])
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                pair_count++;

                printf("(%i,%i) ", pairs[k].winner, pairs[k].loser);

                k++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[k].winner = j;
                pairs[k].loser = i;
                pair_count++;

                printf("(%i,%i) ", pairs[k].winner, pairs[k].loser);

                k++;
            }
        }
    }

    printf("\n");

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int largest;
    int position;
    pair dummy = {0, 0};

    // Iterate for each pair
    for (int i = 0; i < pair_count; i++)
    {
        position = -1;
        largest = strength(i);

        for (int j = i + 1; j < pair_count; j++)
        {
            if (strength(j) > largest)
            {
                largest = strength(j);
                position = j;
            }
        }

        if (position != -1)
        {
            dummy = pairs[position];
            pairs[position] = pairs[i];
            pairs[i] = dummy;
        }
    }

    return;
}

int strength(n)
{
    int victory_strength;

    if (preferences[pairs[n].winner][pairs[n].loser] > preferences[pairs[n].loser][pairs[n].winner])
    {
        victory_strength = preferences[pairs[n].winner][pairs[n].loser] - preferences[pairs[n].loser][pairs[n].winner];
    }
    else
    {
        victory_strength = preferences[pairs[n].loser][pairs[n].winner] - preferences[pairs[n].winner][pairs[n].loser];
    }

    return victory_strength;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int locked_pairs = 0;

    // loop for each pair
    for (int i = 0; i < pair_count; i++)
    {
        // if the pair doesnt create a cycle, lock
        if (cycle(pairs[i].winner, pairs[i].loser) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            locked_pairs++;

            printf("(%i,%i)", pairs[i].winner, pairs[i].loser);
        }
    }
    printf("\n");

    for (int k = 0; k < candidate_count; k++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[k][j] == true)
                printf("1 ");
            else
                printf("0 ");
        }

        printf("\n");
    }

    return;
}

bool cycle(winner, loser)
{
    if (winner == loser)
        return true;
    // iterating through each pair
    for (int i = 0; i < pair_count; i++)
    {
        if (loser == pairs[i].winner)
        {
            if (locked[pairs[i].winner][pairs[i].loser] == true)
            {
                if (cycle(winner, pairs[i].loser) == true)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int counter = 0;
    int winner;
    int winner_count = 0;

    // loop for each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // If it has not already been recorded as a winner

        // If the candidate has never lost
        if (lost_check(i) == false)
        {
            if (won_check(i) == true)
            {
                winner = i;
                winner_count++;
            }
        }
    }

    if (winner_count == 1)
    {
        printf("%s\n", candidates[winner]);
    }
    return;
}

bool won_check(i)
{

    for (int k = 0; k < candidate_count; k++)
    {
        if (locked[i][k] == true)
        {
            return true;
        }
    }
    return false;
}

bool lost_check(i)
{
    int lost_counter = 0;

    for (int k = 0; k < candidate_count; k++)
    {
        if (locked[k][i] == true)
        {
            lost_counter++;
        }
    }

    if (lost_counter == 0)
        return false;
    else
        return true;
}