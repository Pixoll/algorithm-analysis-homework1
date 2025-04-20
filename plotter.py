import os
from collections.abc import Callable, Iterable
from pathlib import Path
from typing import Union

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pandas.core.arrays import ExtensionArray
from scipy import optimize

type ArrayLike = Union[ExtensionArray, np.ndarray]
type PlotData = np.ndarray[tuple[int, ...], np.dtype]
type FitFunc = Callable[[PlotData], PlotData] | np.poly1d


class FitData:
    def __init__(self, name: str, x: ArrayLike, y: ArrayLike, x_trend: PlotData):
        self.name = name
        self.x = x
        self.y = y
        self.x_trend = x_trend
        self.fit_func: FitFunc = lambda _x: _x
        self.equation = ""
        self.type = ""


def nlogn_function(x: PlotData, a: np.float64, b: np.float64) -> PlotData:
    """n*log(n) function for curve fitting."""
    return a * x * np.log2(x) + b


def nlog2n_function(x: PlotData, a: np.float64, b: np.float64) -> PlotData:
    """n*log²(n) function for curve fitting."""
    return a * x * np.log2(x) ** 2 + b


def process_csv_file(file_path: Path) -> FitData:
    """Process a single CSV file and create plots. Returns fit data for combined plot."""
    base_filename = os.path.splitext(os.path.basename(file_path))[0]

    df = pd.read_csv(file_path)

    plt.figure(figsize=(21, 14))

    plt.subplot2grid((4, 4), (0, 0), 2, 2)
    plt.errorbar(df["n"], df["t_mean"], yerr=df["t_stdev"], fmt="o-", capsize=5,
                 ecolor="red", color="blue", markersize=5)
    plt.title(f"Mean Execution Time with Standard Deviation - {base_filename}", fontsize=14)
    plt.xlabel("Number of Elements (n)", fontsize=12)
    plt.ylabel("Mean Time (ns)", fontsize=12)
    plt.grid(True, linestyle="--", alpha=0.7)

    plt.subplot2grid((4, 4), (0, 2), 2, 2)
    plt.plot(df["n"], df["t_Q0"], "o-", label="Minimum (Q0)")
    plt.plot(df["n"], df["t_Q1"], "s-", label="Q1")
    plt.plot(df["n"], df["t_Q2"], "^-", label="Median (Q2)")
    plt.plot(df["n"], df["t_Q3"], "d-", label="Q3")
    plt.plot(df["n"], df["t_Q4"], "x-", label="Maximum (Q4)")
    plt.title(f"Quartile Values vs. Number of Elements - {base_filename}", fontsize=14)
    plt.xlabel("Number of Elements (n)", fontsize=12)
    plt.ylabel("Time (ns)", fontsize=12)
    plt.legend()
    plt.grid(True, linestyle="--", alpha=0.7)

    plt.subplot2grid((4, 4), (2, 1), 2, 2)
    plt.scatter(df["n"], df["t_mean"], s=80, c="blue", alpha=0.7)

    x_trend = np.linspace(df["n"].min(), df["n"].max(), 100)
    fit_data = FitData(base_filename, df["n"].values, df["t_mean"].values, x_trend)

    match base_filename.lower():
        case "divide_and_conquer":
            try:
                params: Iterable[np.float64] = optimize.curve_fit(nlog2n_function, df["n"], df["t_mean"])[0]
                a, b = params

                y_trend = nlog2n_function(x_trend, a, b)

                plt.plot(x_trend, y_trend, "r--", linewidth=2)

                model_equation = f"{a:.4f} · nlog₂(n)² + {b:.2f}"
                model_type = "nlog²(n) fit"

                fit_data.fit_func = lambda x: nlog2n_function(x, a, b)
                fit_data.equation = model_equation
                fit_data.type = model_type

            except Exception as e:
                print(f"Error fitting nlog₂(n)² model: {e}. Falling back to polynomial fit.")
                z = np.polyfit(df["n"], df["t_mean"], 2)
                p = np.poly1d(z)
                y_trend = p(x_trend)
                plt.plot(x_trend, y_trend, "r--", linewidth=2)
                model_equation = f"{z[0]:.4f}n² + {z[1]:.2f}n + {z[2]:.2f}"
                model_type = "Polynomial fit"

                fit_data.fit_func = p
                fit_data.equation = model_equation
                fit_data.type = model_type

        case "divide_and_conquer_improved":
            try:
                params: Iterable[np.float64] = optimize.curve_fit(nlogn_function, df["n"], df["t_mean"])[0]
                a, b = params

                y_trend = nlogn_function(x_trend, a, b)

                plt.plot(x_trend, y_trend, "r--", linewidth=2)

                model_equation = f"{a:.4f} · nlog₂(n) + {b:.2f}"
                model_type = "nlog(n) fit"

                fit_data.fit_func = lambda x: nlogn_function(x, a, b)
                fit_data.equation = model_equation
                fit_data.type = model_type

            except Exception as e:
                print(f"Error fitting nlog₂(n) model: {e}. Falling back to polynomial fit.")
                z = np.polyfit(df["n"], df["t_mean"], 2)
                p = np.poly1d(z)
                y_trend = p(x_trend)
                plt.plot(x_trend, y_trend, "r--", linewidth=2)
                model_equation = f"{z[0]:.4f}n² + {z[1]:.2f}n + {z[2]:.2f}"
                model_type = "Polynomial fit"

                fit_data.fit_func = p
                fit_data.equation = model_equation
                fit_data.type = model_type

        case _:
            z = np.polyfit(df["n"], df["t_mean"], 2)
            p = np.poly1d(z)
            y_trend = p(x_trend)
            plt.plot(x_trend, y_trend, "r--", linewidth=2)
            model_equation = f"{z[0]:.4f}n² + {z[1]:.2f}n + {z[2]:.2f}"
            model_type = "Polynomial fit"

            fit_data.fit_func = p
            fit_data.equation = model_equation
            fit_data.type = model_type

    plt.title(f"Mean Time vs. Number of Elements with {model_type} - {base_filename}", fontsize=14)
    plt.xlabel("Number of Elements (n)", fontsize=12)
    plt.ylabel("Mean Time (ns)", fontsize=12)
    plt.grid(True, linestyle="--", alpha=0.7)

    plt.annotate(model_equation, xy=(0.05, 0.95), xycoords="axes fraction",
                 fontsize=10, bbox=dict(boxstyle="round,pad=0.3", fc="white", ec="gray", alpha=0.8))

    plt.tight_layout(w_pad=2, h_pad=2)

    output_dir = "plots"
    os.makedirs(output_dir, exist_ok=True)

    output_path = os.path.join(output_dir, f"{base_filename}_analysis.png")
    plt.savefig(output_path, dpi=300)
    plt.close()  # Close the figure to free memory

    print(f"Plot saved as '{output_path}'")

    return fit_data


def create_combined_fit_plot(all_fits: list[FitData]) -> None:
    """Create a plot combining all fit curves for comparison."""
    plt.figure(figsize=(14, 20))

    x_min = min([min(fit.x) for fit in all_fits])
    x_max = max([max(fit.x) for fit in all_fits])

    y_max = max([max(fit.y) for fit in all_fits])

    y_max_with_buffer = y_max * 1.1

    x_combined = np.linspace(x_min, x_max, 200)

    plt.subplot(2, 1, 1)

    # noinspection PyUnresolvedReferences
    colors: list[tuple[float, float, float]] = plt.cm.tab10.colors
    for i, fit_data in enumerate(all_fits):
        color = colors[i % len(colors)]

        plt.scatter(fit_data.x, fit_data.y, s=50, alpha=0.4, color=color,
                    label=f"{fit_data.name} (data)")

        y_fit = fit_data.fit_func(x_combined)
        plt.plot(x_combined, y_fit, linestyle="-", linewidth=2, color=color,
                 label=f"{fit_data.name}: {fit_data.equation}")

    plt.title("Comparison of Algorithm Performance Fit Curves With Data Points", fontsize=16)
    plt.xlabel("Number of Elements (n)", fontsize=14)
    plt.ylabel("Execution Time (ns)", fontsize=14)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.ylim(0, y_max_with_buffer)
    plt.legend(fontsize=10, loc="best")

    plt.subplot(2, 1, 2)

    for fit_data in all_fits:
        y_fit = fit_data.fit_func(x_combined)
        plt.plot(x_combined, y_fit, linestyle="-", linewidth=2, label=f"{fit_data.name}: {fit_data.equation}")

    plt.title("Comparison of Algorithm Performance Fit Curves", fontsize=16)
    plt.xlabel("Number of Elements (n)", fontsize=14)
    plt.ylabel("Execution Time (ns)", fontsize=14)
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.ylim(0, y_max_with_buffer)
    plt.legend(fontsize=10, loc="best")

    output_dir = "plots"
    os.makedirs(output_dir, exist_ok=True)

    output_path = os.path.join(output_dir, "combined_fit_curves.png")
    plt.tight_layout(w_pad=2, h_pad=2)
    plt.savefig(output_path, dpi=300)
    plt.close()

    print(f"Combined fit curves plot saved as '{output_path}'")


def main():
    """Process all CSV files in the data directory."""
    data_dir = "data"

    if not os.path.exists(data_dir):
        print(f"Error: Directory '{data_dir}' not found.")
        return

    csv_files = list(Path(data_dir).glob("*.csv"))

    if not csv_files:
        print(f"No CSV files found in '{data_dir}' directory.")
        return

    print(f"Found {len(csv_files)} CSV files to process.")

    all_fits: list[FitData] = []

    for file_path in csv_files:
        try:
            print(f"Processing {file_path}...")
            fit_data = process_csv_file(file_path)
            all_fits.append(fit_data)
        except Exception as e:
            print(f"Error processing {file_path}: {e}")

    create_combined_fit_plot(all_fits)

    print("Processing complete.")


if __name__ == "__main__":
    main()
