import tkinter as tk
from tkinter import ttk

class PixelGrid:
    def __init__(self, root):
        self.root = root
        self.root.title("Font Paint Tool")

        self.rows = 8
        self.cols = 8
        self.cell_size = 40

        self.canvas = None
        self.grid = []
        self.rects = []

        self.create_controls()
        self.create_grid()

    def create_controls(self):
        controls = ttk.Frame(self.root)
        controls.pack(padx=10, pady=10)

        ttk.Label(controls, text="Width:").grid(row=0, column=0)
        self.width_var = tk.StringVar(value="8")
        ttk.Entry(controls, textvariable=self.width_var, width=5).grid(
            row=0, column=1, padx=5
        )

        ttk.Label(controls, text="Height:").grid(row=0, column=2)
        self.height_var = tk.StringVar(value="8")
        ttk.Entry(controls, textvariable=self.height_var, width=5).grid(
            row=0, column=3, padx=5
        )

        ttk.Button(
            controls,
            text="Create Grid",
            command=self.create_grid
        ).grid(row=0, column=4, padx=5)

        ttk.Button(
            controls,
            text="Clear",
            command=self.clear_grid
        ).grid(row=0, column=5, padx=5)

    def create_grid(self):
        try:
            self.cols = int(self.width_var.get())
            self.rows = int(self.height_var.get())

            if self.cols <= 0 or self.rows <= 0:
                raise ValueError

        except ValueError:
            return

        if self.canvas:
            self.canvas.destroy()

        self.grid = [
            [False for _ in range(self.cols)]
            for _ in range(self.rows)
        ]

        width = self.cols * self.cell_size
        height = self.rows * self.cell_size

        self.canvas = tk.Canvas(
            self.root,
            width=width,
            height=height,
            background="white",
            highlightthickness=1
        )
        self.canvas.pack(padx=10, pady=10)

        self.rects = []

        for row in range(self.rows):
            rect_row = []

            for col in range(self.cols):
                x1 = col * self.cell_size
                y1 = row * self.cell_size
                x2 = x1 + self.cell_size
                y2 = y1 + self.cell_size

                rect = self.canvas.create_rectangle(
                    x1,
                    y1,
                    x2,
                    y2,
                    fill="white",
                    outline="black"
                )

                self.canvas.tag_bind(
                    rect,
                    "<Button-1>",
                    lambda event, r=row, c=col: self.toggle(r, c)
                )

                rect_row.append(rect)

            self.rects.append(rect_row)

        self.create_output()

    def toggle(self, row, col):
        self.grid[row][col] = not self.grid[row][col]

        self.canvas.itemconfig(
            self.rects[row][col],
            fill="black" if self.grid[row][col] else "white"
        )

        self.update_output()

    def clear_grid(self):
        for row in range(self.rows):
            for col in range(self.cols):
                self.grid[row][col] = False
                self.canvas.itemconfig(
                    self.rects[row][col],
                    fill="white"
                )

        self.update_output()

    def create_output(self):
        if hasattr(self, "output"):
            self.output.destroy()

        frame = ttk.Frame(self.root)
        frame.pack(fill="x", padx=10, pady=(0, 10))

        ttk.Label(frame, text="Byte array:").pack(anchor="w")

        self.output = tk.Entry(frame, font=("Courier", 11))
        self.output.pack(fill="x")

        self.update_output()

    def update_output(self):
        if not hasattr(self, "output"):
            return

        values = []

        # Each column becomes one byte.
        # Top pixel = bit 0
        # Bottom pixel = bit (rows - 1)
        for col in range(self.cols):
            value = 0

            for row in range(self.rows):
                if self.grid[row][col]:
                    value |= 1 << row

            values.append(f"0x{value:02X}")

        result = "{ " + ", ".join(values) + " }"

        self.output.delete(0, tk.END)
        self.output.insert(0, result)


if __name__ == "__main__":
    root = tk.Tk()
    app = PixelGrid(root)
    root.mainloop()