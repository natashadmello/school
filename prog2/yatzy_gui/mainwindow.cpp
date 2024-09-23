// mainwindow.cpp

// Program author:
// Name: Natasha Dmello

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "functions.hh"
#include "gameengine.hh"
#include <vector>
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>
#include <QProcess>
#include <QInputDialog>
#include <QPushButton>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Dices={
        ui->Die_1,
        ui->Die_2,
        ui->Die_3,
        ui->Die_4,
        ui->Die_5
    };

    // Connect button clicks to corresponding slots.
    connect(ui->EditName, &QToolButton::clicked, this, &MainWindow::enableNameEdit);
    connect(ui->Pause, &QPushButton::clicked, this, &MainWindow::togglePause);

    // Set up and start the timer for updating elapsed time.
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::disableUIElements()
{
    ui->Quit->setDisabled(true);
    ui->Turn->setDisabled(true);
    ui->Roll->setDisabled(true);
    ui->EditName->setDisabled(true);
    ui->Pause->setDisabled(true);
    ui->Lock_1->setDisabled(true);
    ui->Lock_2->setDisabled(true);
    ui->Lock_3->setDisabled(true);
    ui->Lock_4->setDisabled(true);
    ui->Lock_5->setDisabled(true);
}


void MainWindow::updateGameStatus()
{
    // Load dice images for each possible value
    QPixmap one(":dicepictures/1.png");
    QPixmap two(":dicepictures/2.png");
    QPixmap three(":dicepictures/3.png");
    QPixmap four(":dicepictures/4.png");
    QPixmap five(":dicepictures/5.png");
    QPixmap six(":dicepictures/6.png");
    string name=eng.players_.at(eng.game_turn_).Name;

    // Retrieve the current player's name and remaining rolls
    unsigned int rollsLeft=eng.players_.at(eng.game_turn_).rolls_left_;

    // Update UI elements with player information
    ui->curr_player->setText(QString::fromStdString(name));
    ui->Rolls_Left->setText(QString::number(rollsLeft));
    ui->MessageBoard->setText("Player "+QString::fromStdString(name)+" to play next");

    // Determine the total points and result type for the current player
    int totalPoints=eng.players_.at(eng.game_turn_).latestTotalPoints;
    Result resultType = static_cast<Result>(totalPoints);

    // Display the result type if within a valid range, otherwise clear the text
    if(totalPoints>=0 && totalPoints<=7)
        ui->RankOrder->setText("It's "+QString::fromStdString(RESULT_TEXTS.at(resultType)));
    else
        ui->RankOrder->setText("");

     // Update UI elements for individual point categories
    for (auto it = eng.players_.at(eng.game_turn_).RESULT_POINTS.begin(); it != eng.players_.at(eng.game_turn_).RESULT_POINTS.end(); ++it)
    {
        if( it->first==NOTHING)
            ui->NOTHING->setText(QString::number(it->second));
        if( it->first==PAIR)
            ui->PAIR->setText(QString::number(it->second));
        if( it->first==TWO_PAIRS)
            ui->TWO_PAIRS->setText(QString::number(it->second));
        if( it->first==THREE_OF_A_KIND)
            ui->THREE_OF_A_KIND->setText(QString::number(it->second));
        if( it->first==STRAIGHT)
            ui->STRAIGHT->setText(QString::number(it->second));
        if( it->first==FULL_HOUSE)
            ui->FULL_HOUSE->setText(QString::number(it->second));
        if( it->first==FOUR_OF_A_KIND)
            ui->FOUR_OF_A_KIND->setText(QString::number(it->second));
        if( it->first==YATZY)
            ui->YATZY->setText(QString::number(it->second));
    }

    // Update UI elements for locked points on each die
    if(eng.players_.at(eng.game_turn_).lockedPoints[0]==-1)
        ui->Lock_1->setText("Lock");
    else
        ui->Lock_1->setText("Unlock");

    if(eng.players_.at(eng.game_turn_).lockedPoints[1]==-1)
        ui->Lock_2->setText("Lock");
    else
        ui->Lock_2->setText("Unlock");

    if(eng.players_.at(eng.game_turn_).lockedPoints[2]==-1)
        ui->Lock_3->setText("Lock");
    else
        ui->Lock_3->setText("Unlock");

    if(eng.players_.at(eng.game_turn_).lockedPoints[3]==-1)
        ui->Lock_4->setText("Lock");
    else
        ui->Lock_4->setText("Unlock");

    if(eng.players_.at(eng.game_turn_).lockedPoints[4]==-1)
        ui->Lock_5->setText("Lock");
    else
        ui->Lock_5->setText("Unlock");

    // Update UI elements to display dice images based on rolled values
    for (unsigned int i = 0; i < eng.players_.at(eng.game_turn_).latest_point_values_.size(); ++i) {
           switch (eng.players_.at(eng.game_turn_).latest_point_values_[i]) {
               case 1:
                   Dices[i]->setPixmap(one.scaled(80, 80, Qt::KeepAspectRatio));
                   break;
               case 2:
                   Dices[i]->setPixmap(two.scaled(80, 80, Qt::KeepAspectRatio));
                   break;
               case 3:
                   Dices[i]->setPixmap(three.scaled(80, 80, Qt::KeepAspectRatio));
                   break;
               case 4:
                   Dices[i]->setPixmap(four.scaled(80, 80, Qt::KeepAspectRatio));
                   break;
               case 5:
                   Dices[i]->setPixmap(five.scaled(80, 80, Qt::KeepAspectRatio));
                   break;
               case 6:
                   Dices[i]->setPixmap(six.scaled(80, 80, Qt::KeepAspectRatio));
                   break;
           }
       }
}

// Function to edit player's name
void MainWindow::enableNameEdit()
{
    QString newName = QInputDialog::getText(this, tr("Update Name"), tr("Enter your updated name: "));
    if (!newName.isEmpty()) {
        eng.players_.at(eng.game_turn_).Name=newName.toStdString();

    }
    updateGameStatus();

}


void MainWindow::setNumberOfPlayers(unsigned int players)
{
    ui->labelNumberOfPlayers->setText("Number of Players: " + QString::number(players));

     // Initialize and add players to the game engine
    for(unsigned int i = 0; i < players; ++i)
    {
        // Create a new player with default values
        Player player;
        player.id_=i+1;
        player.rolls_left_=INITIAL_NUMBER_OF_ROLLS;
        player.latest_point_values_={};
        player.best_point_values_={};
        player.Name=std::to_string(i+1);

        // Add the initialized player to the game engine
        eng.add_player(player);
    }
     updateGameStatus();

}


void MainWindow::on_Roll_clicked()
{
    vector<int> points;
    string result;

    // Roll the dice using the game engine
    points=eng.roll();

     // Check if there are no more rolls left
    if(points[0]==404)
    {
        paused = !paused;
        QMessageBox::warning(this, "No More Rolls", "No more rolls left. Please proceed to the next player.");
        paused = !paused;
           return;
    }

    // Check if the game has ended
    if(points[0]==200)
    {
            paused = !paused;
            // Disable UI elements
            disableUIElements();

            // Display winner information and update UI
            string message=eng.report_winner();
            ui->MessageBoard->setText(QString::fromStdString(message));
            QPalette palette;
            palette.setColor(QPalette::Window, QColor(135, 206, 235));
            this->setPalette(palette);
            QMessageBox::information(this, "Winner!!!",QString::fromStdString(message));
            return;
    }
     updateGameStatus();
}

void MainWindow::on_Turn_clicked()
{
    int res=eng.give_turn();
    if(res==1)
    {
        paused = !paused;
        // Disable UI elements
        disableUIElements();

        // Update game status and display winner information
        updateGameStatus();
        string message=eng.report_winner();
        ui->MessageBoard->setText(QString::fromStdString(message));
        QPalette palette;
        palette.setColor(QPalette::Window, QColor(135, 206, 235));
        this->setPalette(palette);
        QMessageBox::information(this, "Winner!!!",QString::fromStdString(message));
    }

    // Clear the displayed dice images
    ui->Die_1->clear();
    ui->Die_2->clear();
    ui->Die_3->clear();
    ui->Die_4->clear();
    ui->Die_5->clear();
     updateGameStatus();
}

void MainWindow::on_Quit_clicked()
{
    // Disable UI elements
    disableUIElements();

    paused = !paused;

    // Get and display winner information
    string message=eng.report_winner();
    ui->MessageBoard->setText(QString::fromStdString(message));

    // Set background color and show winner message box
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(135, 206, 235));
    this->setPalette(palette);
    QMessageBox::information(this, "Winner!!!",QString::fromStdString(message));

}

void MainWindow::updateTime()
{
    // If the game is not paused, increment elapsed time in seconds
    if (!paused) {
        ++elapsedTimeInSeconds;

        // Calculate hours, minutes, and seconds
        int hours = elapsedTimeInSeconds / 3600;
        int minutes = (elapsedTimeInSeconds % 3600) / 60;
        int seconds = elapsedTimeInSeconds % 60;

        // Format time as HH:MM:SS
        QString timeString = QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

        // Set the formatted time as the text of the UI element
        ui->myTimer->setText(timeString);
    }
}

void MainWindow::togglePause()
{
    paused = !paused;

    if (paused) {
        // UI updates for paused state
        ui->Pause->setText("Resume");
        ui->Quit->setDisabled(true);
        ui->Turn->setDisabled(true);
        ui->Roll->setDisabled(true);
        ui->EditName->setDisabled(true);
        ui->Lock_1->setDisabled(true);
        ui->Lock_2->setDisabled(true);
        ui->Lock_3->setDisabled(true);
        ui->Lock_4->setDisabled(true);
        ui->Lock_5->setDisabled(true);

    } else {
        // UI updates for resumed state
        ui->Pause->setText("Pause");
        ui->Quit->setEnabled(true);
        ui->Turn->setEnabled(true);
        ui->Roll->setEnabled(true);
        ui->EditName->setEnabled(true);
        ui->Lock_1->setEnabled(true);
        ui->Lock_2->setEnabled(true);
        ui->Lock_3->setEnabled(true);
        ui->Lock_4->setEnabled(true);
        ui->Lock_5->setEnabled(true);

    }
}

void MainWindow::on_Reset_clicked()
{
    this->close();

    // Restart the application using QProcess
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

}


void MainWindow::on_Lock_1_clicked()
{
    // Check if the player has rolled the dic
    if(eng.players_.at(eng.game_turn_).latest_point_values_.size()>0)
    {
        // Check if the first die is locked or unlocked
        if(eng.players_.at(eng.game_turn_).lockedPoints[0]==-1)
        {
            // Lock the first die
            eng.players_.at(eng.game_turn_).lockedPoints[0]=eng.players_.at(eng.game_turn_).latest_point_values_[0];
            ui->Lock_1->setText("Unlock");
        }
        else
        {
            // Unlock the first die
            eng.players_.at(eng.game_turn_).lockedPoints[0]=-1;
            ui->Lock_1->setText("Lock");
        }
    }
    // Display a message if the player hasn't rolled the die before attempting to lock
    else QMessageBox::information(nullptr, "Information", "Roll the die first before locking.");
}


void MainWindow::on_Lock_2_clicked()
{
    if(eng.players_.at(eng.game_turn_).latest_point_values_.size()>0)
    {
        if(eng.players_.at(eng.game_turn_).lockedPoints[1]==-1)
        {
            eng.players_.at(eng.game_turn_).lockedPoints[1]=eng.players_.at(eng.game_turn_).latest_point_values_[1];
            ui->Lock_2->setText("Unlock");
        }
        else
        {
            eng.players_.at(eng.game_turn_).lockedPoints[1]=-1;
            ui->Lock_2->setText("Lock");
        }
    }
    else QMessageBox::information(nullptr, "Information", "Roll the die first before locking.");
}


void MainWindow::on_Lock_3_clicked()
{
    if(eng.players_.at(eng.game_turn_).latest_point_values_.size()>0)
    {
        if(eng.players_.at(eng.game_turn_).lockedPoints[2]==-1)
        {
            eng.players_.at(eng.game_turn_).lockedPoints[2]=eng.players_.at(eng.game_turn_).latest_point_values_[2];
            ui->Lock_3->setText("Unlock");
        }
        else
        {
            eng.players_.at(eng.game_turn_).lockedPoints[2]=-1;
            ui->Lock_3->setText("Lock");
        }
    }
    else QMessageBox::information(nullptr, "Information", "Roll the die first before locking.");
}


void MainWindow::on_Lock_4_clicked()
{
    if(eng.players_.at(eng.game_turn_).latest_point_values_.size()>0)
    {
        if(eng.players_.at(eng.game_turn_).lockedPoints[3]==-1)
        {
            eng.players_.at(eng.game_turn_).lockedPoints[3]=eng.players_.at(eng.game_turn_).latest_point_values_[3];
            ui->Lock_4->setText("Unlock");
        }
        else
        {
            eng.players_.at(eng.game_turn_).lockedPoints[3]=-1;
            ui->Lock_4->setText("Lock");
        }
    }
    else QMessageBox::information(nullptr, "Information", "Roll the die first before locking.");
}


void MainWindow::on_Lock_5_clicked()
{
    if(eng.players_.at(eng.game_turn_).latest_point_values_.size()>0)
    {
        if(eng.players_.at(eng.game_turn_).lockedPoints[4]==-1)
        {
            eng.players_.at(eng.game_turn_).lockedPoints[4]=eng.players_.at(eng.game_turn_).latest_point_values_[4];
            ui->Lock_5->setText("Unlock");
        }
        else
        {
            eng.players_.at(eng.game_turn_).lockedPoints[4]=-1;
            ui->Lock_5->setText("Lock");
        }
    }
    else QMessageBox::information(nullptr, "Information", "Roll the die first before locking.");
}

// Closes the main window
void MainWindow::on_Close_clicked()
{
    this->close();
}


