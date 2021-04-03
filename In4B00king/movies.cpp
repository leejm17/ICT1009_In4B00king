/*
    Objective: Source file for Movie classes.
    Author: Lee Jun Ming (1802971).
*/

#include <movies.h>

/* Retrieves a list of movies & duration from DB & append to their respective arrays. */
void MovieListInfo::getMovieList_Db() {
    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare(
        "SELECT name, duration"
        " FROM MovieList"
        " WHERE (debut <= DATE() AND finale >= DATE());"
    );

    if(!query.exec()) {
        qDebug() << query.lastError().text() << query.lastQuery();
    } else {
        qDebug() << "getMovieList_Db() read query for all movies was successful.";
        while(query.next()) {
            movieNameList.append(query.value(0).toString());
            movieDurationList.append(query.value(1).toInt());
        }
        MyDB::ResetInstance();
    }
}

/* (1) Call displayMovieList() in MainScreen_Cust to display an array/series of movies currently on show. */
void MovieListInfo::displayMovieList(QList<QString> movieNameList, QList<int> movieDurationList) {
    getMovieList_Db();
    // movieNameList array stores a list of movies
    // movieDurationList stores a list of movies' duration
    for(int cnt=0; cnt<movieNameList.size(); cnt++) {
        qDebug() << movieNameList.at(cnt);
        qDebug() << movieDurationList.at(cnt);
    }
}

/* (2) When Customer clicks on a Movie in MainScreen, via this Constructor, initialise the data members. */
/* Calls getMovieDetails_Db() to retrieve description & array of movieDates, then initialise them. */
MovieInfo::MovieInfo(QString movieName, int duration) {
    this->movieName = movieName;
    this->movieDuration = duration;
    getMovieDetails_Db();
}

/* When Admin creates a new movie in MainScreen_Admin, via this Constructor, initialise the data members. */
MovieInfo::MovieInfo(QString movieName, int duration, QString debut, QString finale, QString desc) {
    this->movieName = movieName;
    this->movieDuration = duration;
    this->movieDebut = debut;
    this->movieFinale = finale;
    this->movieDesc = desc;
    /* SELECT MovieList.desc, MovieShowing.date FROM MovieList WHERE (MovieList.name=movieName)
        ON (MovieList.movieId = MovieShowing.movieId). */
}

/* Call getMovieDetails_Db() in MovieInfo() Constructor. */
void MovieInfo::getMovieDetails_Db() {
    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare(
        "SELECT duration, debut, desc FROM MovieList"
        " WHERE (name=:name);"
    );
    query.bindValue(":name", this->movieName);

    if(!query.exec()) {
        qDebug() << query.lastError().text() << query.lastQuery();
    } else {
        qDebug() << "getMovieDetails_Db() read query for " << this->movieName
                 << " was successful.";
        while(query.next()) {
            this->movieDuration = query.value(0).toInt();
            this->movieDebut = query.value(1).toString();
            this->movieDesc = query.value(2).toString();
        }
        MyDB::ResetInstance();
    }
}

/* Call generateMovieDates() in MainScreen_Admin to generate an incremental list of movie show dates & append to movieDates[]. */
void MovieInfo::generateMovieDates() {

}

/* (4) Call displayMovieDetails() in MovieDetails to display a movie's information with its corresponding timeslots based on a given day. */
void displayMovieDetails(MovieInfo, ShowtimesInfo) {

}

/* addMovie_Db() adds the movie record into Database tables MovieList, MovieShowing, MovieSeats. */
void addMovie_Db(MovieInfo movie, ShowtimesInfo showtime) {
    QSqlQuery query(MyDB::getInstance()->getDBInstance());

    /* (1) INSERT movie details INTO MovieList */
    query.prepare(
        "INSERT INTO MovieList (name, duration, debut, finale, description)"
        " VALUES (:name, :duration, :debut, :finale, :desc);"
    );
    query.bindValue(":name", movie.movieName);
    query.bindValue(":date", movie.movieDuration);
    query.bindValue(":debut", movie.movieDebut);
    query.bindValue(":finale", movie.movieFinale);
    query.bindValue(":desc", movie.movieDesc);

    if(!query.exec()) {
        qDebug() << query.lastError().text() << query.lastQuery();
    } else {
        qDebug() << "addMovie() into MovieList write query for " << movie.movieName
                 << " was successful.";
    }

    /* (2) INSERT details INTO MovieShowing */
    /* (2a) SELECT movie_ID of Movie to add */
    query.prepare(
        "SELECT Movie_ID"
        " FROM MovieList"
        " WHERE (name=:name);"
    );
    query.bindValue(":name", movie.movieName);
    if(!query.exec()) {
        qDebug() << query.lastError().text() << query.lastQuery();
    } else {
        qDebug() << "addMovie() read query for " << movie.movieName
                 << " Movie_ID was successful.";
        while(query.next()) {
            movie.movieID = query.value(0).toInt();
        }
    }

    /* (2b) For every Date and their respective Timeslot,
     * INSERT the date, timeslots and halls INTO MovieShowing */
    for (int dateCnt=0; dateCnt<movie.movieDates.length(); dateCnt++) {
        for (int slotCnt=0; slotCnt<showtime.timeslots.length(); slotCnt++) {
            query.prepare(
                "INSERT INTO MovieShowing (movie_ID, date, timeslot, hall_ID)"
                " VALUES (:id, :date, :timeslot, :hall)"
            );
            query.bindValue(":id", showtime.movieID);
            query.bindValue(":date", showtime.movieDates[dateCnt]);
            query.bindValue(":timeslot", showtime.timeslots[slotCnt]);
            query.bindValue(":hall", showtime.halls[slotCnt]);
            if(!query.exec()) {
                qDebug() << query.lastError().text() << query.lastQuery();
            } else {
                qDebug() << "addMovie() into MovieShowing write query for " << showtime.movieName
                         << " on " << showtime.movieDates[dateCnt]
                         << " at " << showtime.timeslots[slotCnt]
                         << " was successful.";
            }
        }
    }

    /* (3) INSERT INTO MovieSeats to make Seats available for booking */
    /* (3a) SELECT show_ID and their respective seat_ID & seat's condition based on hall_ID seating plan */
    QList<int> showIDs;
    QList<int> seatIDs;
    QList<QString> seatsCondition;
    query.prepare(
        "SELECT MovieShowing.show_ID, HallSeats.seat_ID, HallSeats.condition"
        " FROM MovieShowing"
        " JOIN HallSeats ON (MovieShowing.hall_ID = HallSeats.hall_ID)"
        " WHERE (movie_ID=:id);"
    );
    query.bindValue(":id", movie.movieID);
    if(!query.exec()) {
        qDebug() << query.lastError().text() << query.lastQuery();
    } else {
        qDebug() << "addMovie() read query for " << movie.movieName
                 << " Show_ID, Hall_ID and Seat_ID was successful.";
        while(query.next()) {
            showIDs.append(query.value(0).toInt());
            seatIDs.append(query.value(1).toInt());
            seatsCondition.append(query.value(3).toString());
        }
    }

    /* (3b) For every record, if seatsCondition is NOT 'good', replace it with 'FALSE', else 'TRUE' */
    for (int cnt=0; cnt<seatsCondition.length(); cnt++) {
        if (seatsCondition[cnt] != "good") {
            seatsCondition.replace(cnt, "FALSE");
        } else {
            seatsCondition.replace(cnt, "TRUE");
        }

        /* (3c) INSERT seat_ID & condition for the respective show_ID based on (3a) INTO MovieSeats */
        query.prepare(
            "INSERT INTO MovieSeats (show_ID, seat_ID, available)"
            " VALUES (:show, :seat, :avail);"
        );
        query.bindValue(":show", showIDs[cnt]);
        query.bindValue(":seat", seatIDs[cnt]);
        query.bindValue(":avail", seatsCondition[cnt]);
        if(!query.exec()) {
            qDebug() << query.lastError().text() << query.lastQuery();
        } else {
            qDebug() << "addMovie() into MovieSeats write query for " << movie.movieName
                     << " Show_ID, Hall_ID and Seat_ID was successful.";
            while(query.next()) {
                showIDs.append(query.value(0).toInt());
                seatIDs.append(query.value(1).toInt());
                seatsCondition.append(query.value(3).toString());
            }
        }
    }

    /* INSERT INTO Hall (type)
        VALUES ('normal' OR 'diamond') */

    MyDB::ResetInstance();
}

/* (3) When Customer clicks on a movie date, call getShowtimes_Db(). */
/* Retrieves a list of timeslots & hallIDs from DB, and append to their respective arrays. */
void ShowtimesInfo::getShowtimes_Db(QString movieName, QString movieDate) {
    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("SELECT timeslot, hall_ID FROM MovieShowing"
                  " JOIN MovieList ON (MovieShowing.movie_ID = MovieList.movie_ID)"
                  " WHERE (MovieList.name=:name AND MovieShowing.date=:date);");
    query.bindValue(":name", movieName);
    query.bindValue(":date", movieDate);

    if(!query.exec()) {
        qDebug() << query.lastError().text() << query.lastQuery();
    } else {
        qDebug() << "getShowtimes_Db() read query for " << movieName
                 << " on " << movieDate
                 << " was successful.";
        while(query.next()) {
            timeslots.append(query.value(0).toString());
            halls.append(query.value(1).toInt());
        }
        MyDB::ResetInstance();
    }
}

/* Call generateShowtimes() in MainScreen_Admin to generate a list of movie show times & match their halls, then append to their respective arrays. */
void ShowtimesInfo::generateShowtimes(QString) {

}
