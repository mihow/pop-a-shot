import time
import curses

HIGH = 1
LOW  = 0
COL_WIDTH = 23
GAME_SECONDS = 30



class Game(object):
    time = 0
    score = 0
    playing = False

    def __init__(self, stdscr, machine_id, sensor_pin, 
                 button1_pin, button2_pin, timer_display, score_display):
        self.stdscr = stdscr
        self.machine_id = machine_id
        self.sensor_pin = sensor_pin
        self.button1_pin = button1_pin
        self.button2_pin = button2_pin
        self.timer_display = timer_display
        self.score_display = score_display
        self.debug(2, "Press '%s' to start" % self.button1_pin)

    def start(self):
        self.stdscr.clear()
        self.time = 0
        self.score = 0
        self.playing = True
        self.start_time = time.time()

    def stop(self):
        self.stop_time = self.time
        self.playing = False
        self.debug(5, "Game Over!")

    def update(self, current_time):
        if self.playing:
            self.check_sensor()
            self.update_timer(current_time)
        else:
            self.check_buttons()

    def get_input(self):
        try:
            return self.stdscr.getkey()
        except Exception:
            return None

    def check_sensor(self):
        key = self.get_input()
        if self.sensor_pin == key:
            self.increment_score()

    def check_buttons(self):
        key = self.get_input()

        if self.button1_pin == key:
            self.start()

        if self.button2_pin == key:
            # multi player
            pass

    def update_timer(self, current_time):
        elapsed_time = int(current_time - self.start_time)
        
        if (elapsed_time - self.time) >= 1:
            # Increment clock every second
            self.time = elapsed_time 

            # update timer display 
            msg = "Time: %s" % self.time
            self.debug(3, msg)

        if (elapsed_time >= GAME_SECONDS):
            self.stop()

    def increment_score(self):
        self.score += 2

        # update score display
        msg = "Score: %s" % self.score
        self.debug(4, msg)

    def debug(self, row, msg):
        # print("Machine:", self.machine_id, msg)

        col = self.machine_id * COL_WIDTH
        self.stdscr.addstr(0, col, "Machine: %s" % self.machine_id)
        self.stdscr.addstr(row, col, msg)



def main(stdscr):
    stdscr.clear()

    machines = [
        Game(stdscr, 1, 'q', '1', '7', 4, 5),
        Game(stdscr, 2, 'w', '2', '8', 4, 5),
        Game(stdscr, 3, 'e', '3', '9', 4, 5),
    ]

    curses.noecho()
    curses.cbreak()
    curses.curs_set(False)
    stdscr.nodelay(True)

    while True:
        current_time = time.time()

        for machine in machines:
            machine.update(current_time)

        stdscr.refresh()


if __name__ == '__main__':
    curses.wrapper(main)
