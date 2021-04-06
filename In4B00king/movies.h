/*
    Objective: Header file for Movie classes.
    Author: Lee Jun Ming (1802971).
*/
#ifndef MOVIES_H
#define MOVIES_H

#include <QMainWindow>
#include <QMessageBox>
#include <QComboBox>
#include "mydb.h"

/* Forward declaration for friend-function. */
//class ShowtimesInfo;

struct Movie
{
    QLabel* title;
    QLabel* duration;
};

/* Class to store movie information for a list of movies (Screen: MainScreen). */
class MovieListInfo {
private:
    QList<int> movieIDList;         // stores an array of movie IDs in the same order as the two lists below (from DB)
    QList<QString> movieNameList;    // stores an array of movie names in same order as movieDurationList[] (from DB)
    QList<QString> movieDurationList; // stores an array of movie duration in same order as movieNameList[] (from DB)
public:
    MovieListInfo();    // default constructor
    void getMovieList_Db();     // getMovies for customer where date range of movie is within current date
    void displayMovieList(Movie*); // Movie is a struct defined in movielist.h, but passing over a struct array
    void getAllMovieList_Db();  // getAllMovies for admin
    QList<QString> getMovieNameList();  // getter to get movieNameList
};


/* Class to store movie information for any 1 movie (Screen: MainScreen_Admin). */
class MovieInfo {
private:
    int movieID;    // store movieID from DB
    QString movieName;
    int movieDuration;
    QString movieDebut;  // first date to show movie
    QString movieFinale;   // last date to show movie
    QString movieDesc;   // movie synopsis
    QList<QString> movieDates;    // generate an array of dates from movieDebut to movieFinale

    int priority;
    QList<QString> economy_timeslots;   // stores an array of timeslots for a given movie on a given day in same order as halls[] (from DB)
    QList<int> economy_halls;           // stores an array of halls the cinema has in same order as timeslots[] (from DB)
    QList<QString> diamond_timeslots;   // similar to economy_timeslots, but for diamond halls
    QList<int> diamond_halls;           // similar to economy_halls, but for diamond halls
public:
    MovieInfo();    // default constructor
    MovieInfo(QString, int);  // movieName, movieDuration
    void getMovieDetails_Db();

    MovieInfo(QString, int, QString, QString, QString); // movieName, movieDuration, movieDebut, movieFinale, movieDesc
    void generateMovieDates();
    void appendMovieDate(int, int, int);    // year, month, date
    void getPriority_Db();
    void generateShowtimes();
    void addMovie_Db();   // movieName, movieDuration, movieDebut, movieFinale, movieDesc, movieDates, timeslots
    void deleteMovie_Db(QString);  // movieName

    /* Getters & Setters */
    MovieInfo(QString);     // movieName
	int getMovieID();
    QString getMovieName();
    int getMovieDuration();
    QString getMovieDebut();
    QString getMovieDesc();
    void updateMovieDetails();
    MovieInfo(QString, QString, int, QString);  // oldMovieName, newMovieName, duration, desc
};


/* Class to store showtimes for any 1 movie based on a given date (Screen: MovieDetails & MainScreen_Admin). */
class ShowtimesInfo {
private:
    QString movieName;
    QString movieDate;
    QList<QString> retrieve_timeslots;
    QList<int> retrieve_halls;
    QList<QString> retrieve_dates;
public:
    ShowtimesInfo() {};   // unused default constructor
    void displayMovieDetails(QString, QString);  // movieName, movieDate
    void getShowtimes_Db(QString, QString); // movieName, movieDate

    // 2nd
    void displayMovieDetails(QString, QComboBox*);  // movieName, movieDate
    void getShowtimes_Db(QString); // movieName, movieDate
};


class MovieSelection : ShowtimesInfo {
private:
    QString selectedTimeslot;
    int selectedHall;
public:
    /* (5) When Customer clicks on movie timeslot, via this constructor, initialise data members, then load the hall layout. */
    /* Calls displayLayout() based on hallID (eg if hallID<10, normal layout; if hallID>10, diamond layout) */
    //MovieSelection(QString, QString, QString, int);    // movieName, movieDate, timeslot, hall
};

#endif
