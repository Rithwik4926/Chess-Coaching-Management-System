// THE WHOLE PROGRAM IS WRITTEN ASSUMING THAT THE NAMES OF EACH STUDENT OR EACH TRAINER IS UNIQUE AS PEOPLE WITH SAME NAMES HAVE DIFFERENT SURNAMES MOST OF THE TIME.
#include <stdio.h>
#include <string.h> // For string functions
#include <limits.h> //For using INT_MAX

#define MAX_SIZE 100           // Maximum no of students or trainers
#define NAME_SIZE 100          // Maximum length of names
#define MAX_GOALS 3            // e.g., Opening Strategies, Endgame Techniques
#define MAX_AVAILABILITY 3     // Maximum 5 time slots can be available for student or teacher
#define MAX_STYLES 3           // aggressive defensive positional
#define MAX_PERFORMANCE_DATA 3 // weak areas like tactics, endgames, etc.
#define MAX_RATING_HISTORY 12  // For 12 months

typedef struct
{
    int student_serial_no; // For first student in list it is 1, For second student in list it is 2 etc
    char student_name[NAME_SIZE];
    int ELO_rating; // above 1400
    char learning_goals[MAX_GOALS][NAME_SIZE];
    char availability[MAX_AVAILABILITY][NAME_SIZE];         // e.g., Monday-09(monday 9a.m.) Tuesday-18(tuesday 6p.m.)
    char preferred_coaching_styles[MAX_STYLES][NAME_SIZE];  // Should be given in preference order from choices aggressive/positional/defensive
    unsigned long current_trainer_id;                       // id of the trainer who is training the student
    char performance_data[MAX_PERFORMANCE_DATA][NAME_SIZE]; // weak areas like tactics, endgames, etc.

    struct
    {
        int games_played;
        int games_won;
        int puzzles_solved;
        int rating_changes[MAX_RATING_HISTORY];
    } progress;

} Student;

typedef struct
{
    unsigned long trainer_id;
    char trainer_name[NAME_SIZE];
    int ELO_rating;
    char coaching_style[NAME_SIZE];                 // One of aggressive/positional/defensive
    char availability[MAX_AVAILABILITY][NAME_SIZE]; // e.g., Monday-09(monday 9a.m.) Tuesday-18(tuesday 6p.m.)
    int experience;                                 // Experience in years
    int min_ELO_rating;                             // Minimum ELO rating a student must have to take training from the trainer
    int max_students;                               // Maximum number of students a trainer can train at once
    int current_students;                           // Current number of students taking training
    char student_names[MAX_SIZE][NAME_SIZE];        // List of names students trained by the trainer
    int student_serial_no[MAX_SIZE];                // List of serial numbers of student a trainer is training
} Trainer;

Student students[MAX_SIZE];
Trainer trainers[MAX_SIZE];
int student_count = 0;
int trainer_count = 0;                              // Global variables and arrays are assigned

void append_students_to_file()
{
    FILE *file = fopen("student_data_input.txt", "w");
    if (file == NULL)
    {
        printf("Error opening student data file for writing!\n");
        return;
    }
    int i = student_count;
    {
        fprintf(file, "%d\n%s\n%d\n", students[i].student_serial_no, students[i].student_name, students[i].ELO_rating);
        for (int j = 0; j < MAX_GOALS; j++)
        {
            fprintf(file, "%s\n", students[i].learning_goals[j]);
        }
        for (int j = 0; j < MAX_AVAILABILITY; j++)
        {
            fprintf(file, "%s\n", students[i].availability[j]);
        }
        for (int j = 0; j < MAX_STYLES; j++)
        {
            fprintf(file, "%s\n", students[i].preferred_coaching_styles[j]);
        }
        fprintf(file, "%lu\n", students[i].current_trainer_id);
        for (int j = 0; j < MAX_PERFORMANCE_DATA; j++)
        {
            fprintf(file, "%s\n", students[i].performance_data[j]);
        }
        fprintf(file, "%d\n%d\n%d\n", students[i].progress.games_played, students[i].progress.games_won, students[i].progress.puzzles_solved);
        for (int j = 0; j < MAX_RATING_HISTORY; j++)
        {
            fprintf(file, "%d\n", students[i].progress.rating_changes[j]);
        }
    }
    fclose(file);
}

void append_trainers_to_file()
{
    FILE *file = fopen("trainer_data_input.txt", "w");
    if (file == NULL)
    {
        printf("Error opening trainer data file for writing!\n");
        return;
    }
    int i = trainer_count;
    {
        fprintf(file, "%lu\n%s\n%d\n%s\n", trainers[i].trainer_id, trainers[i].trainer_name, trainers[i].ELO_rating, trainers[i].coaching_style);
        for (int j = 0; j < MAX_AVAILABILITY; j++)
        {
            fprintf(file, "%s\n", trainers[i].availability[j]);
        }
        fprintf(file, "%d\n%d\n%d\n%d\n", trainers[i].experience, trainers[i].min_ELO_rating, trainers[i].max_students, trainers[i].current_students);
        for (int j = 0; j < trainers[i].current_students; j++)
        {
            fprintf(file, "%s\n%d\n", trainers[i].student_names[j], trainers[i].student_serial_no[j]);
        }
    }
    fclose(file);
}

void save_trainers_in_output_file()
{
    FILE *file = fopen("output_trainer.txt", "w");
    if (file == NULL)
    {
        printf("Error opening trainer data file for writing!\n");
        return;
    }
    for (int i = 0; i < trainer_count; i++)
    {
        fprintf(file, "%lu\n%s\n%d\n%s\n", trainers[i].trainer_id, trainers[i].trainer_name, trainers[i].ELO_rating, trainers[i].coaching_style);
        for (int j = 0; j < MAX_AVAILABILITY; j++)
        {
            fprintf(file, "%s\n", trainers[i].availability[j]);
        }
        fprintf(file, "%d\n%d\n%d\n%d\n", trainers[i].experience, trainers[i].min_ELO_rating, trainers[i].max_students, trainers[i].current_students);
        for (int j = 0; j < trainers[i].current_students; j++)
        {
            fprintf(file, "%s\n%d\n", trainers[i].student_names[j], trainers[i].student_serial_no[j]);
        }
    }
    fclose(file);
}

void save_students_in_output_file()
{
    FILE *file = fopen("output_student.txt", "w");
    if (file == NULL)
    {
        printf("Error opening student data file for writing!\n");
        return;
    }
    for (int i = 0; i < student_count; i++)
    {
        fprintf(file, "%d\n%s\n%d\n", students[i].student_serial_no, students[i].student_name, students[i].ELO_rating);
        for (int j = 0; j < MAX_GOALS; j++)
        {
            fprintf(file, "%s\n", students[i].learning_goals[j]);
        }
        for (int j = 0; j < MAX_AVAILABILITY; j++)
        {
            fprintf(file, "%s\n", students[i].availability[j]);
        }
        for (int j = 0; j < MAX_STYLES; j++)
        {
            fprintf(file, "%s\n", students[i].preferred_coaching_styles[j]);
        }
        fprintf(file, "%lu\n", students[i].current_trainer_id);
        for (int j = 0; j < MAX_PERFORMANCE_DATA; j++)
        {
            fprintf(file, "%s\n", students[i].performance_data[j]);
        }
        fprintf(file, "%d\n%d\n%d\n", students[i].progress.games_played, students[i].progress.games_won, students[i].progress.puzzles_solved);
        for (int j = 0; j < MAX_RATING_HISTORY; j++)
        {
            fprintf(file, "%d\n", students[i].progress.rating_changes[j]);
        }
    }
    fclose(file);
}

void load_students_from_file()
{
    FILE *file = fopen("student_data_input.txt", "r");
    if (file == NULL)
    {
        printf("Error opening student data file for reading!\n");
        return;
    }
    while (fscanf(file, "%d\n", &students[student_count].student_serial_no) != EOF)
    {
        fgets(students[student_count].student_name, NAME_SIZE, file);
        students[student_count].student_name[strcspn(students[student_count].student_name, "\n")] = 0;
        fscanf(file, "%d\n", &students[student_count].ELO_rating);
        for (int j = 0; j < MAX_GOALS; j++)
        {
            fgets(students[student_count].learning_goals[j], NAME_SIZE, file);
            students[student_count].learning_goals[j][strcspn(students[student_count].learning_goals[j], "\n")] = 0;
        }
        for (int j = 0; j < MAX_AVAILABILITY; j++)
        {
            fgets(students[student_count].availability[j], NAME_SIZE, file);
            students[student_count].availability[j][strcspn(students[student_count].availability[j], "\n")] = 0;
        }
        for (int j = 0; j < MAX_STYLES; j++)
        {
            fgets(students[student_count].preferred_coaching_styles[j], NAME_SIZE, file);
            students[student_count].preferred_coaching_styles[j][strcspn(students[student_count].preferred_coaching_styles[j], "\n")] = 0;
        }
        fscanf(file, "%lu\n", &students[student_count].current_trainer_id);
        for (int j = 0; j < MAX_PERFORMANCE_DATA; j++)
        {
            fgets(students[student_count].performance_data[j], NAME_SIZE, file);
            students[student_count].performance_data[j][strcspn(students[student_count].performance_data[j], "\n")] = 0;
        }
        fscanf(file, "%d\n%d\n%d\n", &students[student_count].progress.games_played, &students[student_count].progress.games_won, &students[student_count].progress.puzzles_solved);
        for (int j = 0; j < MAX_RATING_HISTORY; j++)
        {
            fscanf(file, "%d\n", &students[student_count].progress.rating_changes[j]);
        }
        student_count++;
    }
    fclose(file);
}

void load_trainers_from_file()
{
    FILE *file = fopen("trainer_data_input.txt", "r");
    if (file == NULL)
    {
        printf("Error opening trainer data file for reading!\n");
        return;
    }
    while (fscanf(file, "%lu\n", &trainers[trainer_count].trainer_id) != EOF)
    {
        fgets(trainers[trainer_count].trainer_name, NAME_SIZE, file);
        trainers[trainer_count].trainer_name[strcspn(trainers[trainer_count].trainer_name, "\n")] = 0; // Remove newline character
        fscanf(file, "%d\n", &trainers[trainer_count].ELO_rating);
        fgets(trainers[trainer_count].coaching_style, NAME_SIZE, file);
        trainers[trainer_count].coaching_style[strcspn(trainers[trainer_count].coaching_style, "\n")] = 0;
        for (int j = 0; j < MAX_AVAILABILITY; j++)
        {
            fgets(trainers[trainer_count].availability[j], NAME_SIZE, file);
            trainers[trainer_count].availability[j][strcspn(trainers[trainer_count].availability[j], "\n")] = 0;
        }
        fscanf(file, "%d\n%d\n%d\n%d\n", &trainers[trainer_count].experience, &trainers[trainer_count].min_ELO_rating, &trainers[trainer_count].max_students, &trainers[trainer_count].current_students);
        for (int j = 0; j < trainers[trainer_count].current_students; j++)
        {
            fgets(trainers[trainer_count].student_names[j], NAME_SIZE, file);
            trainers[trainer_count].student_names[j][strcspn(trainers[trainer_count].student_names[j], "\n")] = 0;
            fscanf(file, "%d\n", &trainers[trainer_count].student_serial_no[j]);
        }
        trainer_count++;
    }
    fclose(file);
}

void add_student(Student *students, int i)
{
    students[i].student_serial_no = student_count + 1;

    printf("Enter student name with surname\n");
    getchar(); // Consume leftover newline from previous input
    fgets(students[i].student_name, NAME_SIZE, stdin);
    students[i].student_name[strcspn(students[i].student_name, "\n")] = 0; // Remove the newline character from fgets

    printf("Enter ELO Rating of the student\n");
    scanf("%d", &students[i].ELO_rating);
    getchar(); // Consume the leftover newline from scanf

    printf("Enter Learning Goals of the student (%d no of goals)\n", MAX_GOALS);
    for (int j = 0; j < MAX_GOALS; j++)
    {
        fgets(students[i].learning_goals[j], NAME_SIZE, stdin);
        students[i].learning_goals[j][strcspn(students[i].learning_goals[j], "\n")] = 0;
    }

    printf("Enter Available Slots of the student (%d no of slots)\n", MAX_AVAILABILITY);
    for (int j = 0; j < MAX_AVAILABILITY; j++)
    {
        fgets(students[i].availability[j], NAME_SIZE, stdin);
        students[i].availability[j][strcspn(students[i].availability[j], "\n")] = 0;
    }

    printf("Enter the Preferred Coaching Style of student in order\n");
    for (int j = 0; j < MAX_STYLES; j++)
    {
        fgets(students[i].preferred_coaching_styles[j], NAME_SIZE, stdin);
        students[i].preferred_coaching_styles[j][strcspn(students[i].preferred_coaching_styles[j], "\n")] = 0;
    }

    students[i].current_trainer_id = 0; // Initially trainer is not assigned to the student so, trainer id = 0

    printf("Enter the Performance Data of the student\n");
    for (int j = 0; j < MAX_PERFORMANCE_DATA; j++)
    {
        fgets(students[i].performance_data[j], NAME_SIZE, stdin);
        students[i].performance_data[j][strcspn(students[i].performance_data[j], "\n")] = 0;
    }

    printf("Enter the number of games played by the student\n");
    scanf("%d", &students[i].progress.games_played);

    printf("Enter the number of games won by the student\n");
    scanf("%d", &students[i].progress.games_won);

    printf("Enter the number of puzzles solved by the student\n");
    scanf("%d", &students[i].progress.puzzles_solved);

    printf("Enter the monthly change of ELO Rating for 1 year (enter it for %d no of months starting from 1st month)\n", MAX_RATING_HISTORY);
    for (int j = 0; j < MAX_RATING_HISTORY; j++)
    {
        scanf("%d", &students[i].progress.rating_changes[j]);
    }

    student_count++;
    append_students_to_file(); // Save updated students to file
}

void add_trainer(Trainer *trainers, int i)
{
    printf("Enter the Trainer ID\n");
    scanf("%lu", &trainers[i].trainer_id);
    getchar(); // Consume leftover newline character

    printf("Enter the Trainer name with surname\n");
    getchar();
    fgets(trainers[i].trainer_name, NAME_SIZE, stdin);
    trainers[i].trainer_name[strcspn(trainers[i].trainer_name, "\n")] = 0; // Remove the newline character

    printf("Enter the ELO Rating of trainer\n");
    scanf("%d", &trainers[i].ELO_rating);
    getchar(); // Consume leftover newline character

    printf("Enter the Coaching Style of the trainer\n");
    fgets(trainers[i].coaching_style, NAME_SIZE, stdin);
    trainers[i].coaching_style[strcspn(trainers[i].coaching_style, "\n")] = 0; // Remove the newline character

    printf("Enter Available Slots of the trainer (%d no of slots)\n", MAX_AVAILABILITY);
    for (int j = 0; j < MAX_AVAILABILITY; j++)
    {
        fgets(trainers[i].availability[j], NAME_SIZE, stdin);
        trainers[i].availability[j][strcspn(trainers[i].availability[j], "\n")] = 0; // Remove the newline character
    }

    printf("Enter the Experience of trainer in years\n");
    scanf("%d", &trainers[i].experience);

    printf("Enter the min ELO Rating a student must have to train under the trainer\n");
    scanf("%d", &trainers[i].min_ELO_rating);

    printf("Enter the max number of students a trainer can train\n");
    scanf("%d", &trainers[i].max_students);

    trainers[i].current_students = 0; // Initially no students are assigned

    for (int j = 0; j < trainers[i].current_students; j++)
    {
        trainers[i].student_names[j][0] = '\0'; // Since no students, no student names
        trainers[i].student_serial_no[j] = 0;   // Since no students, no student serial numbers
    }

    trainer_count++;
    append_trainers_to_file(); // Save updated trainers to file
}

void delete_student()
{
    char delete_student_name[NAME_SIZE];
    getchar();
    printf("Enter the name of the student to be deleted\n");
    fgets(delete_student_name, NAME_SIZE, stdin);
    delete_student_name[strcspn(delete_student_name, "\n")] = 0; // Remove the newline character that fgets adds
    int found = 0, i = 0;
    while (i < student_count && found == 0)
    {
        if (strcmp(students[i].student_name, delete_student_name) == 0)
        {
            found = 1;
            for (int j = i; j < student_count - 1; j++)
            {
                students[j] = students[j + 1]; // Shifting all the elements to left
            }
            student_count--;
        }
        i++;
    }
    if (found == 0)
    {
        printf("Student entered is not found\n");
    }
    save_students_in_output_file(); // Save updated students to file
}

void delete_trainer()
{
    char delete_trainer_name[NAME_SIZE];
    printf("Enter the name of the trainer to be deleted\n");
    getchar();
    fgets(delete_trainer_name, NAME_SIZE, stdin);
    delete_trainer_name[strcspn(delete_trainer_name, "\n")] = 0; // Remove the newline character that fgets adds
    int found = 0, i = 0;
    while (i < trainer_count && found == 0)
    {
        if (strcmp(trainers[i].trainer_name, delete_trainer_name) == 0)
        {
            found = 1;
            for (int j = i; j < trainer_count - 1; j++)
            {
                trainers[j] = trainers[j + 1]; // Shifting all the elements to left
            }
            trainer_count--;
        }
        i++;
    }
    if (found == 0)
    {
        printf("Trainer entered is not found\n");
    }
    save_trainers_in_output_file(); // Save updated trainers to file
}

void list_students_with_each_trainer()
{
    for (int i = 0; i < trainer_count; i++)
    {
        printf("The list of students with trainer ");
        puts(trainers[i].trainer_name);
        for (int j = 0; j < trainers[i].current_students; j++)
        {
            puts(trainers[i].student_names[j]);
        }
        printf("\n");
        printf("---------------------------------------------------------------------------------\n");
    }
}

void sort_students()
{
    Student temp;
    int flag = 1, temp_serial_no, i = 0;
    while (i < student_count - 1 && flag == 1)
    {
        flag = 0;
        for (int j = 0; j < student_count - i - 1; j++)
        {
            if ((students[j].ELO_rating < students[j + 1].ELO_rating) || ((students[j].ELO_rating == students[j + 1].ELO_rating) && (students[j].progress.games_won < students[j + 1].progress.games_won)))
            {
                temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
                temp_serial_no = students[j].student_serial_no;
                students[j].student_serial_no = students[j + 1].student_serial_no;
                students[j + 1].student_serial_no = temp_serial_no;
                flag = 1;
            }
        }
        i++;
    }
    save_students_in_output_file();
    for (int i = 0; i < student_count; i++)
    {
        printf("Name : ");
        puts(students[i].student_name);
        printf("ELO rating : %d\n", students[i].ELO_rating);
        printf("---------------------------------------------------------------------------------\n");
    }
}

void sort_most_popular_trainer()
{
    Trainer most_popular, temp;
    int flag = 1, i = 0;
    while (i < trainer_count - 1 && flag == 1)
    {
        flag = 0;
        for (int j = 0; j < trainer_count - i - 1; j++)
        {
            float average_ELO_rating_1 = 0, average_ELO_rating_2 = 0;
            if (trainers[j].current_students == trainers[j + 1].current_students)
            {
                int total_ELO_rating_1 = 0, total_ELO_rating_2 = 0;
                for (int k = 0; k < trainers[j].current_students; k++)                                 //---
                {                                                                                      //  |
                    total_ELO_rating_1 += students[(trainers[j].student_serial_no[k]) - 1].ELO_rating; //  |-- To fing average ELO rating of 1st set of students
                } //  |
                average_ELO_rating_1 = (float)(total_ELO_rating_1) / (trainers[j].current_students); //---

                for (int k = 0; k < trainers[j + 1].current_students; k++)
                {
                    total_ELO_rating_2 += students[(trainers[j + 1].student_serial_no[k]) - 1].ELO_rating;
                }
                average_ELO_rating_2 = (float)(total_ELO_rating_2) / (trainers[j + 1].current_students);
            }
            if ((trainers[j].current_students < trainers[j + 1].current_students) || ((trainers[j].current_students == trainers[j + 1].current_students) && (average_ELO_rating_1 < average_ELO_rating_2)))
            {
                temp = trainers[j];
                trainers[j] = trainers[j + 1];
                trainers[j + 1] = temp;
                flag = 1;
            }
        }
        i++;
    }
    // Till here the array is sorted in decreasing order of trainer popularity
    save_trainers_in_output_file();
    most_popular = trainers[0];
    printf("The most popular trainer is ");
    puts(most_popular.trainer_name);
    printf("----------------------------------------------------------------------------------------\n");
    printf("The sorted descending order of trainers on basis of popularity is:\n");
    for (int i = 0; i < trainer_count; i++)
    {
        puts(trainers[i].trainer_name);
    }
}

void sort_strongest_trainer()
{
    Trainer strongest, temp;
    int flag = 1, i = 0;
    while (i < trainer_count - 1 && flag == 1)
    {
        flag = 0;
        for (int j = 0; j < trainer_count - i - 1; j++)
        {
            int total_ELO_rating_1 = 0, total_ELO_rating_2 = 0;
            for (int k = 0; k < trainers[j].current_students; k++)                                 //---
            {                                                                                      //  |
                total_ELO_rating_1 += students[(trainers[j].student_serial_no[k]) - 1].ELO_rating; //  |-- To fing average ELO rating of 1st set of students
            } //  |
            float average_ELO_rating_1 = (float)(total_ELO_rating_1) / (trainers[j].current_students); //---

            for (int k = 0; k < trainers[j + 1].current_students; k++)
            {
                total_ELO_rating_2 += students[(trainers[j + 1].student_serial_no[k]) - 1].ELO_rating;
            }
            float average_ELO_rating_2 = (float)(total_ELO_rating_2) / (trainers[j + 1].current_students);

            if (average_ELO_rating_1 < average_ELO_rating_2)
            {
                temp = trainers[j];
                trainers[j] = trainers[j + 1];
                trainers[j + 1] = temp;
                flag = 1;
            }
        }
        i++;
    }
    // Till here the array is sorted and strongest trainer is the first element in the array
    save_trainers_in_output_file();
    strongest = trainers[0];
    printf("The strongest trainer is ");
    puts(strongest.trainer_name);
    printf("----------------------------------------------------------------------------------------------\n");
    printf("The sorted descending order of trainers on basis of strength is:\n");
    for (int i = 0; i < trainer_count; i++)
    {
        puts(trainers[i].trainer_name);
    }
}

void assign(Trainer *trainer, Student *student)
{
    student->current_trainer_id = trainer->trainer_id;
    strcpy(trainer->student_names[(trainer->current_students)], student->student_name);
    trainer->student_serial_no[(trainer->current_students)] = student->student_serial_no;
    trainer->current_students += 1;
}

void assign_trainer()
{
    char name[NAME_SIZE];
    printf("Enter the name of the student desired to be assigned to a trainer\n");
    getchar();
    fgets(name, NAME_SIZE, stdin);
    name[strcspn(name, "\n")] = 0;
    Student student_to_be_assigned;
    int found = 0, l = 0;
    while (l < student_count && found == 0)
    {
        if (strcmp(students[l].student_name, name) == 0)
        {
            student_to_be_assigned = students[l];
            found = 1;
        }
        l++;
    }
    if (found == 0)
    {
        printf("Student not found\n");
        return;
    }
    int student_index = (student_to_be_assigned.student_serial_no) - 1, found_trainer = 0;
    int k = 0;
    while (k < 3 && found_trainer == 0)
    {
        int i = 0;
        while (i < trainer_count && found_trainer == 0)
        {
            if (trainers[i].current_students < trainers[i].max_students)
            {
                int availability_match = 0;
                for (int j = 0; j < MAX_AVAILABILITY; j++)
                {
                    for (int m = 0; m < MAX_AVAILABILITY; m++)
                    {
                        if (strcmp(students[student_index].availability[j], trainers[i].availability[m]) == 0)
                        {
                            availability_match = 1;
                        }
                    }
                }
                if (availability_match == 1)
                {
                    if (students[student_index].ELO_rating >= trainers[i].min_ELO_rating)
                    {
                        if (strcmp(students[student_index].preferred_coaching_styles[k], trainers[i].coaching_style) == 0)
                        {
                            assign(&trainers[i], &students[student_index]);
                            found_trainer = 1;
                            printf("The best possible trainer for give student is ");
                            puts(trainers[i].trainer_name);
                        }
                    }
                }
            }
            i++;
        }
        k++;
    }

    if (found_trainer == 0)
    {
        int trainer_location = -1, k = 0;
        while (k < 3 && found_trainer == 0)
        {
            int min_difference = INT_MAX;
            for (int i = 0; i < trainer_count; i++)
            {
                if (trainers[i].current_students < trainers[i].max_students)
                {
                    int availability_match = 0;
                    for (int j = 0; j < MAX_AVAILABILITY; j++)
                    {
                        for (int m = 0; m < MAX_AVAILABILITY; m++)
                        {
                            if (strcmp(students[student_index].availability[j], trainers[i].availability[m]) == 0)
                            {
                                availability_match = 1;
                            }
                        }
                    }
                    if (availability_match == 1)
                    {
                        if (strcmp(students[student_index].preferred_coaching_styles[k], trainers[i].coaching_style) == 0)
                        {
                            if ((trainers[i].min_ELO_rating - students[student_index].ELO_rating) < min_difference)
                            {
                                min_difference = (trainers[i].min_ELO_rating - students[student_index].ELO_rating);
                                trainer_location = i;
                            }
                        }
                    }
                }
            }
            if (trainer_location != -1)
            {
                assign(&trainers[trainer_location], &students[student_index]);
                found_trainer = 1;
                printf("The best possible trainer for give student is ");
                puts(trainers[trainer_location].trainer_name);
            }
            k++;
        }
    }

    if (found_trainer == 0)
    {
        printf("There is no trainer with available time slots to teach the student or all the trainers have reached their maximum strength of students or both\n");
    }
    save_students_in_output_file();
    save_trainers_in_output_file();
}

void sort_consistent_improving_ELO_ratings()
{
    // Making an array of students whose elo ratings are consisently increasing for last 12 months
    Student improving_students[MAX_SIZE];
    int improving_students_count = 0;
    for (int i = 0; i < student_count; i++)
    {
        int j = 1, ELO_increasing = 1;
        while (j < MAX_RATING_HISTORY && ELO_increasing == 1)
        {
            if (students[i].progress.rating_changes[j] <= students[i].progress.rating_changes[j - 1])
            {
                ELO_increasing = 0;
            }
            j++;
        }

        if (ELO_increasing == 1 && students[i].progress.rating_changes[MAX_RATING_HISTORY] < students[i].ELO_rating)
        {
            improving_students[improving_students_count] = students[i];
            improving_students_count++;
        }
    }
    // Sorting the array of the students whose elo ratings are consisently increasing for last 12 months
    Student temp;
    int flag;
    for (int i = 0; i < improving_students_count - 1; i++)
    {
        flag = 0;
        for (int j = 0; j < improving_students_count - i - 1; j++)
        {
            if ((improving_students[j].ELO_rating - improving_students[j].progress.rating_changes[0]) < (improving_students[j + 1].ELO_rating - improving_students[j + 1].progress.rating_changes[0]))
            {
                temp = improving_students[j];
                improving_students[j] = improving_students[j + 1];
                improving_students[j + 1] = temp;
                flag = 1;
            }
        }
        if (flag == 0)
        {
            return;
        }
    }
    // Printing the sorted list of students in descending order
    printf("Students whose ELO ratings increased every month for the past on year are:\n");
    for (int i = 0; i < improving_students_count; i++)
    {
        printf("Name : ");
        puts(improving_students[i].student_name);
        printf("ELO gain : %d\n", (improving_students[i].ELO_rating - improving_students[i].progress.rating_changes[0]));
        printf("----------------------------------------------------------------------------------------------\n");
    }
}

void print_students_list()
{
    for (int i = 0; i < student_count; i++)
    {
        printf("Serial no : %d\n", students[i].student_serial_no);
        printf("Student name : ");
        puts(students[i].student_name);
        printf("ELO rating : %d\n", students[i].ELO_rating);
        printf("Learning goals are : \n");
        for (int j = 0; j < MAX_GOALS; j++)
        {
            puts(students[i].learning_goals[j]);
        }
        printf("The available slots of sudent are : \n");
        for (int j = 0; j < MAX_AVAILABILITY; j++)
        {
            puts(students[i].availability[j]);
        }
        printf("The preffered coaching styles in order are : \n");
        for (int j = 0; j < MAX_STYLES; j++)
        {
            puts(students[i].preferred_coaching_styles[j]);
        }
        printf("The current trainer id is %lu\n", students[i].current_trainer_id);
        printf("The performance data of the student is :\n");
        for (int j = 0; j < MAX_PERFORMANCE_DATA; j++)
        {
            puts(students[i].performance_data[j]);
        }
        printf("No of games played are : %d\n", students[i].progress.games_played);
        printf("No of games won are : %d\n", students[i].progress.games_won);
        printf("No of puzzles solved are : %d\n", students[i].progress.puzzles_solved);
        printf("The rating history of the student is :\n");
        for (int j = 0; j < MAX_RATING_HISTORY; j++)
        {
            printf("%d  ", students[i].progress.rating_changes[j]);
        }
        printf("\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    }
}

void print_trainers_list()
{
    for (int i = 0; i < trainer_count; i++)
    {
        printf("Trainer id : %lu\n", trainers[i].trainer_id);
        printf("Trainer name : ");
        puts(trainers[i].trainer_name);
        printf("Elo rating : %d\n", trainers[i].ELO_rating);
        printf("Coaching style of trainer : ");
        puts(trainers[i].coaching_style);
        printf("The available slots of trainer are : \n");
        for (int j = 0; j < MAX_AVAILABILITY; j++)
        {
            puts(trainers[i].availability[j]);
        }
        printf("Experience of the trainer : %d years\n", trainers[i].experience);
        printf("The min Elo rating to students is : %d\n", trainers[i].min_ELO_rating);
        printf("Max no of students are : %d\n", trainers[i].max_students);
        printf("Current no of students : %d\n", trainers[i].current_students);
        printf("The list of current students with names and serial no is :\n");
        for (int j = 0; j < trainers[i].current_students; j++)
        {
            printf("Name : ");
            puts(trainers[i].student_names[j]);
            printf("Serial no : %d\n", trainers[i].student_serial_no[j]);
        }
        printf("\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    }
}

int main()
{
    load_students_from_file(); // Load student data from file at start
    load_trainers_from_file(); // Load trainer data from file at start
    int operation;
    while (operation != 0)
    {
        printf("Enter the operation to be done to the database from the following list\n"
               "1.Add Student\n"
               "2.Add Trainer\n"
               "3.Delete Student\n"
               "4.Delete Trainer\n"
               "5.List set of students with every trainer\n"
               "6.Sort Students based on ELO rating in decreasing order (if rating is same consider number of games won criterion)\n"
               "7.Find the most popular Trainer\n"
               "8.Find the strongest Trainer\n"
               "9.Assign Trainer to a particular Student (by giving the name of the student)\n"
               "10.To find list of students who have successively increased their rating every month for the last one year and sort them in the decreasing order of ELO rating gains over the last year\n"
               "11.To print the list of students\n"
               "12.To print the list of trainers\n"
               "0.To end the operations\n");

        scanf("%d", &operation);
        switch (operation)
        {
        case 0:
            break;

        case 1:
            add_student(students, student_count);
            break;

        case 2:
            add_trainer(trainers, trainer_count);
            break;

        case 3:
            delete_student();
            break;

        case 4:
            delete_trainer();
            break;

        case 5:
            list_students_with_each_trainer();
            break;

        case 6:
            sort_students();
            break;

        case 7:
            sort_most_popular_trainer();
            break;

        case 8:
            sort_strongest_trainer();
            break;

        case 9:

            assign_trainer();
            break;

        case 10:
            sort_consistent_improving_ELO_ratings();
            break;

        case 11:
            print_students_list();
            break;

        case 12:
            print_trainers_list();
            break;

        default:
            printf("Invalid function entered\n");
            break;
        }
    }
    return 0;
}