// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <implot.h>

#include "ImGuiWrapperFunctionLibrary.h"
#include "ImplotWrapperFunctionLibrary.generated.h"

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EImPlotFlags
{
	enum Type {
		ImPlotFlags_None          = 0,       // default
		ImPlotFlags_NoTitle       = 1 << 0,  // the plot title will not be displayed (titles are also hidden if preceeded by double hashes, e.g. "##MyPlot")
		ImPlotFlags_NoLegend      = 1 << 1,  // the legend will not be displayed
		ImPlotFlags_NoMenus       = 1 << 2,  // the user will not be able to open context menus with right-click
		ImPlotFlags_NoBoxSelect   = 1 << 3,  // the user will not be able to box-select with right-click drag
		ImPlotFlags_NoMousePos    = 1 << 4,  // the mouse position, in plot coordinates, will not be displayed inside of the plot
		ImPlotFlags_NoHighlight   = 1 << 5,  // plot items will not be highlighted when their legend entry is hovered
		ImPlotFlags_NoChild       = 1 << 6,  // a child window region will not be used to capture mouse scroll (can boost performance for single ImGui window applications)
		ImPlotFlags_Equal         = 1 << 7,  // primary x and y axes will be constrained to have the same units/pixel (does not apply to auxiliary y-axes)
		ImPlotFlags_YAxis2        = 1 << 8,  // enable a 2nd y-axis on the right side
		ImPlotFlags_YAxis3        = 1 << 9,  // enable a 3rd y-axis on the right side
		ImPlotFlags_Query         = 1 << 10, // the user will be able to draw query rects with middle-mouse or CTRL + right-click drag
		ImPlotFlags_Crosshairs    = 1 << 11, // the default mouse cursor will be replaced with a crosshair when hovered
		ImPlotFlags_AntiAliased   = 1 << 12, // plot lines will be software anti-aliased (not recommended for high density plots, prefer MSAA)
		ImPlotFlags_CanvasOnly    = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMousePos
	};
}


//UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EImPlotAxisFlags
{
	enum Type {
		ImPlotAxisFlags_None          = 0,       // default
		ImPlotAxisFlags_NoLabel       = 1 << 0,  // the axis label will not be displayed (axis labels also hidden if the supplied string name is NULL)
		ImPlotAxisFlags_NoGridLines   = 1 << 1,  // no grid lines will be displayed
		ImPlotAxisFlags_NoTickMarks   = 1 << 2,  // no tick marks will be displayed
		ImPlotAxisFlags_NoTickLabels  = 1 << 3,  // no text labels will be displayed
		ImPlotAxisFlags_Foreground    = 1 << 4,  // grid lines will be displayed in the foreground (i.e. on top of data) in stead of the background
		ImPlotAxisFlags_LogScale      = 1 << 5,  // a logartithmic (base 10) axis scale will be used (mutually exclusive with ImPlotAxisFlags_Time)
		ImPlotAxisFlags_Time          = 1 << 6,  // axis will display date/time formatted labels (mutually exclusive with ImPlotAxisFlags_LogScale)
		ImPlotAxisFlags_Invert        = 1 << 7,  // the axis will be inverted
		ImPlotAxisFlags_NoInitialFit  = 1 << 8,  // axis will not be initially fit to data extents on the first rendered frame (also the case if SetNextPlotLimits explicitly called)
		ImPlotAxisFlags_AutoFit       = 1 << 9,  // axis will be auto-fitting to data extents
		ImPlotAxisFlags_RangeFit      = 1 << 10, // axis will only fit points if the point is in the visible range of the **orthoganol** axis
		ImPlotAxisFlags_LockMin       = 1 << 11, // the axis minimum value will be locked when panning/zooming
		ImPlotAxisFlags_LockMax       = 1 << 12, // the axis maximum value will be locked when panning/zooming
		ImPlotAxisFlags_Lock          = ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax,
		ImPlotAxisFlags_NoDecorations = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels
	};
}


UENUM(BlueprintType)
enum class EImPlotCol : uint8
{
	// item styling colors
    ImPlotCol_Line,          // plot line/outline color (defaults to next unused color in current colormap)
    ImPlotCol_Fill,          // plot fill color for bars (defaults to the current line color)
    ImPlotCol_MarkerOutline, // marker outline color (defaults to the current line color)
    ImPlotCol_MarkerFill,    // marker fill color (defaults to the current line color)
    ImPlotCol_ErrorBar,      // error bar color (defaults to ImGuiCol_Text)
    // plot styling colors
    ImPlotCol_FrameBg,       // plot frame background color (defaults to ImGuiCol_FrameBg)
    ImPlotCol_PlotBg,        // plot area background color (defaults to ImGuiCol_WindowBg)
    ImPlotCol_PlotBorder,    // plot area border color (defaults to ImGuiCol_Border)
    ImPlotCol_LegendBg,      // legend background color (defaults to ImGuiCol_PopupBg)
    ImPlotCol_LegendBorder,  // legend border color (defaults to ImPlotCol_PlotBorder)
    ImPlotCol_LegendText,    // legend text color (defaults to ImPlotCol_InlayText)
    ImPlotCol_TitleText,     // plot title text color (defaults to ImGuiCol_Text)
    ImPlotCol_InlayText,     // color of text appearing inside of plots (defaults to ImGuiCol_Text)
    ImPlotCol_XAxis,         // x-axis label and tick lables color (defaults to ImGuiCol_Text)
    ImPlotCol_XAxisGrid,     // x-axis grid color (defaults to 25% ImPlotCol_XAxis)
    ImPlotCol_YAxis,         // y-axis label and tick labels color (defaults to ImGuiCol_Text)
    ImPlotCol_YAxisGrid,     // y-axis grid color (defaults to 25% ImPlotCol_YAxis)
    ImPlotCol_YAxis2,        // 2nd y-axis label and tick labels color (defaults to ImGuiCol_Text)
    ImPlotCol_YAxisGrid2,    // 2nd y-axis grid/label color (defaults to 25% ImPlotCol_YAxis2)
    ImPlotCol_YAxis3,        // 3rd y-axis label and tick labels color (defaults to ImGuiCol_Text)
    ImPlotCol_YAxisGrid3,    // 3rd y-axis grid/label color (defaults to 25% ImPlotCol_YAxis3)
    ImPlotCol_Selection,     // box-selection color (defaults to yellow)
    ImPlotCol_Query,         // box-query color (defaults to green)
    ImPlotCol_Crosshairs,    // crosshairs color (defaults to ImPlotCol_PlotBorder)
    ImPlotCol_COUNT
};

UENUM(BlueprintType)
enum class EImPlotStyleVar : uint8
{
    // item styling variables
    ImPlotStyleVar_LineWeight,         // float,  plot item line weight in pixels
    ImPlotStyleVar_Marker,             // int,    marker specification
    ImPlotStyleVar_MarkerSize,         // float,  marker size in pixels (roughly the marker's "radius")
    ImPlotStyleVar_MarkerWeight,       // float,  plot outline weight of markers in pixels
    ImPlotStyleVar_FillAlpha,          // float,  alpha modifier applied to all plot item fills
    ImPlotStyleVar_ErrorBarSize,       // float,  error bar whisker width in pixels
    ImPlotStyleVar_ErrorBarWeight,     // float,  error bar whisker weight in pixels
    ImPlotStyleVar_DigitalBitHeight,   // float,  digital channels bit height (at 1) in pixels
    ImPlotStyleVar_DigitalBitGap,      // float,  digital channels bit padding gap in pixels
    // plot styling variables
    ImPlotStyleVar_PlotBorderSize,     // float,  thickness of border around plot area
    ImPlotStyleVar_MinorAlpha,         // float,  alpha multiplier applied to minor axis grid lines
    ImPlotStyleVar_MajorTickLen,       // ImVec2, major tick lengths for X and Y axes
    ImPlotStyleVar_MinorTickLen,       // ImVec2, minor tick lengths for X and Y axes
    ImPlotStyleVar_MajorTickSize,      // ImVec2, line thickness of major ticks
    ImPlotStyleVar_MinorTickSize,      // ImVec2, line thickness of minor ticks
    ImPlotStyleVar_MajorGridSize,      // ImVec2, line thickness of major grid lines
    ImPlotStyleVar_MinorGridSize,      // ImVec2, line thickness of minor grid lines
    ImPlotStyleVar_PlotPadding,        // ImVec2, padding between widget frame and plot area, labels, or outside legends (i.e. main padding)
    ImPlotStyleVar_LabelPadding,       // ImVec2, padding between axes labels, tick labels, and plot edge
    ImPlotStyleVar_LegendPadding,      // ImVec2, legend padding from plot edges
    ImPlotStyleVar_LegendInnerPadding, // ImVec2, legend inner padding from legend edges
    ImPlotStyleVar_LegendSpacing,      // ImVec2, spacing between legend entries
    ImPlotStyleVar_MousePosPadding,    // ImVec2, padding between plot edge and interior info text
    ImPlotStyleVar_AnnotationPadding,  // ImVec2, text padding around annotation labels
    ImPlotStyleVar_FitPadding,         // ImVec2, additional fit padding as a percentage of the fit extents (e.g. ImVec2(0.1f,0.1f) adds 10% to the fit extents of X and Y)
    ImPlotStyleVar_PlotDefaultSize,    // ImVec2, default size used when ImVec2(0,0) is passed to BeginPlot
    ImPlotStyleVar_PlotMinSize,        // ImVec2, minimum size plot frame can be when shrunk
    ImPlotStyleVar_COUNT
};


UENUM(BlueprintType)//, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EImPlotMarker
{
	enum Type {
		ImPlotMarker_None = -1, // no marker
		ImPlotMarker_Circle = 0,// a circle marker
		ImPlotMarker_Square,    // a square maker
		ImPlotMarker_Diamond,   // a diamond marker
		ImPlotMarker_Up,        // an upward-pointing triangle marker
		ImPlotMarker_Down,      // an downward-pointing triangle marker
		ImPlotMarker_Left,      // an leftward-pointing triangle marker
		ImPlotMarker_Right,     // an rightward-pointing triangle marker
		ImPlotMarker_Cross,     // a cross marker (not fillable)
		ImPlotMarker_Plus,      // a plus marker (not fillable)
		ImPlotMarker_Asterisk,  // a asterisk marker (not fillable)
		ImPlotMarker_COUNT
	};
}


UENUM(BlueprintType)
enum class EImPlotColormap : uint8
{
	ImPlotColormap_Deep = 0,   // a.k.a. seaborn deep             (qual=true,  n=10) (default)
	ImPlotColormap_Dark = 1,   // a.k.a. matplotlib "Set1"        (qual=true,  n=9 )
	ImPlotColormap_Pastel = 2,   // a.k.a. matplotlib "Pastel1"     (qual=true,  n=9 )
	ImPlotColormap_Paired = 3,   // a.k.a. matplotlib "Paired"      (qual=true,  n=12)
	ImPlotColormap_Viridis = 4,   // a.k.a. matplotlib "viridis"     (qual=false, n=11)
	ImPlotColormap_Plasma = 5,   // a.k.a. matplotlib "plasma"      (qual=false, n=11)
	ImPlotColormap_Hot = 6,   // a.k.a. matplotlib/MATLAB "hot"  (qual=false, n=11)
	ImPlotColormap_Cool = 7,   // a.k.a. matplotlib/MATLAB "cool" (qual=false, n=11)
	ImPlotColormap_Pink = 8,   // a.k.a. matplotlib/MATLAB "pink" (qual=false, n=11)
	ImPlotColormap_Jet = 9,   // a.k.a. MATLAB "jet"             (qual=false, n=11)
	ImPlotColormap_Twilight = 10,  // a.k.a. matplotlib "twilight"    (qual=false, n=11)
	ImPlotColormap_RdBu = 11,  // red/blue, Color Brewer          (qual=false, n=11)
	ImPlotColormap_BrBG = 12,  // brown/blue-green, Color Brewer  (qual=false, n=11)
	ImPlotColormap_PiYG = 13,  // pink/yellow-green, Color Brewer (qual=false, n=11)
	ImPlotColormap_Spectral = 14,  // color spectrum, Color Brewer    (qual=false, n=11)
	ImPlotColormap_Greys = 15,  // white/black                     (qual=false, n=2 )
};


UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EImPlotLocation
{
	enum Type {
		ImPlotLocation_Center    = 0,                                          // center-center
		ImPlotLocation_North     = 1 << 0,                                     // top-center
		ImPlotLocation_South     = 1 << 1,                                     // bottom-center
		ImPlotLocation_West      = 1 << 2,                                     // center-left
		ImPlotLocation_East      = 1 << 3,                                     // center-right
		ImPlotLocation_NorthWest = ImPlotLocation_North | ImPlotLocation_West, // top-left
		ImPlotLocation_NorthEast = ImPlotLocation_North | ImPlotLocation_East, // top-right
		ImPlotLocation_SouthWest = ImPlotLocation_South | ImPlotLocation_West, // bottom-left
		ImPlotLocation_SouthEast = ImPlotLocation_South | ImPlotLocation_East  // bottom-right
	};
}

UENUM(BlueprintType)
enum class EImPlotOrientation : uint8
{
    ImPlotOrientation_Horizontal, // left/right
    ImPlotOrientation_Vertical    // up/down
};

UENUM(BlueprintType)
enum class EImPlotYAxis : uint8
{
    ImPlotYAxis_1 = 0, // left (default)
    ImPlotYAxis_2 = 1, // first on right side
    ImPlotYAxis_3 = 2  // second on right side
};


// Enums for different automatic histogram binning methods (k = bin count or w = bin width)
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EImPlotBin
{
	enum Type {
		UE4_ERROR         =  0, //does not have a 0 entry! (This is a problem when the enum is initalized by default)
		ImPlotBin_Sqrt    = -1, // k = sqrt(n)
		ImPlotBin_Sturges = -2, // k = 1 + log2(n)
		ImPlotBin_Rice    = -3, // k = 2 * cbrt(n)
		ImPlotBin_Scott   = -4, // w = 3.49 * sigma / cbrt(n)
	};
}


// UPROPERTY(Category = "Input", EditAnywhere, BlueprintReadWrite, Meta = (Bitmask, BitmaskEnum = "EImPlotFlags"))
// int32 PlayerInputFlags = -1;
// bool GetPlayerInputFlag(const TEnumAsByte<EImPlotFlags::Type> InFlag)


USTRUCT(BlueprintType)
struct FImPlotPoint
{
	GENERATED_USTRUCT_BODY()
	
	ImPlotPoint p;

	FImPlotPoint() { p.x = 0.0f, p.y = 0.0f; }
	FImPlotPoint(float x, float y) { p.x = x, p.y = y; }
	FImPlotPoint(const ImPlotPoint& inP) : p(inP){}

	void SetPoint(const FVector2D& newPoint)
	{
		p.x = newPoint.X;
		p.y = newPoint.Y;
	}
	
	operator ImPlotPoint() const 
	{
		return p;
	}

	FImPlotPoint& operator=(const ImPlotPoint& Other)
	{
		this->p.x = Other.x;
		this->p.y = Other.y;
		return *this;
	}
};


// A range defined by a min/max value. Used for plot axes ranges.
USTRUCT(BlueprintType)
struct FImPlotRange
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max;
	
	FImPlotRange()                         { Min = 0; Max = 0; }
	FImPlotRange(double _min, double _max) { Min = _min; Max = _max; }
	FImPlotRange(const ImPlotRange& p) : Min(p.Min), Max(p.Max) { }
	
	bool Contains(float value) const     { return value >= Min && value <= Max; };
	float Size() const                   { return Max - Min; };	
};

// Combination of two ranges for X and Y axes.
USTRUCT(BlueprintType)
struct FImPlotLimits
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FImPlotRange X;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FImPlotRange Y;

	FImPlotLimits() { }
	FImPlotLimits(const ImPlotLimits& p) : X(p.X), Y(p.Y) { }

	bool Contains(const ImPlotPoint& p) const { return Contains(p.x, p.y); }
	bool Contains(double x, double y) const   { return X.Contains(x) && Y.Contains(y); }
};

UCLASS()
class IMGUI_API UImplotFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	/*Blueprint Function to return the value of ExampleProperty from the struct*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ImPlot", Meta = (ReturnDisplayName = "Point"))	
	static FVector2D GetImPlotPoint(UPARAM(ref) FImPlotPoint& Point) { return FVector2D(Point.p.x, Point.p.y); }

	UFUNCTION(BlueprintCallable, Category = "ImPlot")
	static void SetImPlotPoint(UPARAM(ref) FImPlotPoint& Point, FVector2D NewPoint) {  Point.SetPoint(NewPoint); }
	
};

/**
 * 
 */
UCLASS()
class IMGUI_API UImplotWrapperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	// Special color used to indicate that a color should be deduced automatically.
	static const FLinearColor UE_IMPLOT_AUTO_COL;

	
	UFUNCTION(BlueprintPure, Category = "Implot", Meta = (ReturnDisplayName = "Version"))
	static FString GetImplotVersion() { return IMPLOT_VERSION;}
		
	// Starts a 2D plotting context. If this function returns true, EndPlot() must
	// be called, e.g. "if (BeginPlot(...)) { ... EndPlot(); }". #title_id must
	// be unique. If you need to avoid ID collisions or don't want to display a
	// title in the plot, use double hashes (e.g. "MyPlot##Hidden" or "##NoTitle").
	// If #x_label and/or #y_label are provided, axes labels will be displayed.
	UFUNCTION(BlueprintCallable, Category = "Implot", meta=(AdvancedDisplay = "1", ExpandEnumAsExecs="OutResult"))
	static void BeginPlot(const FString& title_id, TEnumAsByte<EImGuiFlowControl::Type>& OutResult,
	                          FString x_label      = TEXT(""),
	                          FString y_label      = TEXT(""),
	                          FVector2D size       = FVector2D(-1,0),
	                          int32 flags	 = 0,
	                          int32 x_flags  = 0,
	                          int32 y_flags  = 0,
	                          int32 y2_flags = 2,
	                          int32 y3_flags = 2,
	                          FString y2_label      = TEXT(""),
	                          FString y3_label      = TEXT(""))
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImPlot::BeginPlot(
			TCHAR_TO_ANSI(*title_id),
			x_label.Len() > 0 ? TCHAR_TO_ANSI(*x_label) : nullptr,
			y_label.Len() > 0 ?TCHAR_TO_ANSI(*y_label) : nullptr,
			ToImVec2(size),
			flags,
			x_flags,
			y_flags,
			y2_flags,
			y3_flags,
			y2_label.Len() > 0 ? TCHAR_TO_ANSI(*y2_label) : nullptr,
			y3_label.Len() > 0 ? TCHAR_TO_ANSI(*y3_label) : nullptr))
		{
			OutResult = EImGuiFlowControl::Success;
		}
	}
 
	
	
	UFUNCTION(BlueprintCallable, Category = "Implot")
	static void EndPlot() { ImPlot::EndPlot(); }

	// PlotLine

	// Plots a standard 2D line plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta=(AdvancedDisplay = "2"))
	static void PlotLineIntA(const FString& label_id, const TArray<int32>& values, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotLine(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), xscale, x0, offset, sizeof(int32));
	}

	// Plots a standard 2D line plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta=(AdvancedDisplay = "3"))
	static void PlotLineIntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotLine(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), offset, sizeof(int32));	
	}

	// Plots a standard 2D line plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta=(AdvancedDisplay = "2"))
	static void PlotLineFloatA(const FString& label_id, const TArray<float>& values, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotLine(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), xscale, x0, offset, sizeof(float));
	}

	// Plots a standard 2D line plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta=(AdvancedDisplay = "3"))
	static void PlotLineFloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotLine(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), offset, sizeof(float));	
	}

	// PlotScatter

	// Plots a standard 2D scatter plot. Default marker is ImPlotMarker_Circle.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotScatterIntA(const FString& label_id, const TArray<int32>& values, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotScatter(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), xscale, x0, offset, sizeof(int32));
	}

	// Plots a standard 2D scatter plot. Default marker is ImPlotMarker_Circle.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotScatterIntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotScatter(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), offset, sizeof(int32));	
	}

	// Plots a standard 2D scatter plot. Default marker is ImPlotMarker_Circle.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotScatterFloatA(const FString& label_id, const TArray<float>& values, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotScatter(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), xscale, x0, offset, sizeof(float));
	}
	
	// Plots a standard 2D scatter plot. Default marker is ImPlotMarker_Circle.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotScatterFloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotScatter(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), offset, sizeof(float));	
	}

	// PlotStairs

	// Plots a a stairstep graph. The y value is continued constantly from every x position, i.e. the interval [x[i], x[i+1]) has the value y[i].
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AdvancedDisplay = 2))
	static void PlotStairsIntA(const FString& label_id, const TArray<int32>& values, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotStairs(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), xscale, x0, offset, sizeof(int32));
	}

	// Plots a a stairstep graph. The y value is continued constantly from every x position, i.e. the interval [x[i], x[i+1]) has the value y[i].
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AdvancedDisplay = 3))
	static void PlotStairsIntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotStairs(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), offset, sizeof(int32));	
	}

	// Plots a a stairstep graph. The y value is continued constantly from every x position, i.e. the interval [x[i], x[i+1]) has the value y[i].
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AdvancedDisplay = 2))
	static void PlotStairsFloatA(const FString& label_id, const TArray<float>& values, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotStairs(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), xscale, x0, offset, sizeof(float));
	}

	// Plots a a stairstep graph. The y value is continued constantly from every x position, i.e. the interval [x[i], x[i+1]) has the value y[i].
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AdvancedDisplay = 3))
	static void PlotStairsFloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotStairs(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), offset, sizeof(float));	
	}

	// PlotShaded

	// Plots a shaded (filled) region between two lines, or a line and a horizontal reference. Set y_ref to +/-INFINITY for infinite fill extents.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AdvancedDisplay = 2))
	static void PlotShadedIntA(const FString& label_id, const TArray<int32>& values, float y_ref = 0, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotShaded(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), y_ref, xscale, x0, offset, sizeof(int32));
	}

	// Plots a shaded (filled) region between two lines, or a line and a horizontal reference. Set y_ref to +/-INFINITY for infinite fill extents.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AdvancedDisplay = 3))
	static void PlotShadedIntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, float y_ref = 0, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotShaded(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), y_ref, offset, sizeof(int32));	
	}

	// Plots a shaded (filled) region between two lines, or a line and a horizontal reference. Set y_ref to +/-INFINITY for infinite fill extents.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AdvancedDisplay = 4))
	static void PlotShadedIntC(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys1, const TArray<int32>& ys2, int offset = 0)
	{
		check(xs.Num() == ys1.Num() && xs.Num() == ys2.Num());
		ImPlot::PlotShaded(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys1.GetData(), ys2.GetData(), xs.Num(), offset, sizeof(int32));	
	}

	// Plots a shaded (filled) region between two lines, or a line and a horizontal reference. Set y_ref to +/-INFINITY for infinite fill extents.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AdvancedDisplay = 2))
	static void PlotShadedFloatA(const FString& label_id, const TArray<float>& values, float y_ref = 0, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotShaded(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), y_ref, xscale, x0, offset, sizeof(float));
	}

	// Plots a shaded (filled) region between two lines, or a line and a horizontal reference. Set y_ref to +/-INFINITY for infinite fill extents.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta=(AdvancedDisplay = "4"))
	static void PlotShadedFloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, float y_ref = 0, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotShaded(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), y_ref, offset, sizeof(float));	
	}

	// Plots a shaded (filled) region between two lines, or a line and a horizontal reference. Set y_ref to +/-INFINITY for infinite fill extents.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta=(AdvancedDisplay = "4"))
	static void PlotShadedFloatC(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys1, const TArray<float>& ys2, int offset = 0)
	{
		check(xs.Num() == ys1.Num() && xs.Num() == ys2.Num());
		ImPlot::PlotShaded(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys1.GetData(), ys2.GetData(), xs.Num(), offset, sizeof(float));	
	}
	
	// PlotBars

	// Plots a vertical bar graph. #width and #shift are in X units.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotBarsIntA(const FString& label_id, const TArray<int32>& values, float width = 0.67f, float shift = 0.0f, int offset = 0)
	{
		ImPlot::PlotBars(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), width, shift, offset, sizeof(int32));
	}

	// Plots a vertical bar graph. #width and #shift are in X units.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotBarsIntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, float width, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotBars(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), width, offset, sizeof(int32));
	}

	// Plots a vertical bar graph. #width and #shift are in X units.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotBarsFloatA(const FString& label_id, const TArray<float>& values, float width = 0.67f, float shift = 0.0f, int offset = 0)
	{
		ImPlot::PlotBars(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), width, shift, offset, sizeof(float));
	}

	// Plots a vertical bar graph. #width and #shift are in X units.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotBarsFloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, float width, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotBars(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), width, offset, sizeof(float));	
	}

	// PlotBarsH

	// Plots a horizontal bar graph. #height and #shift are in Y units.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotBarsH_IntA(const FString& label_id, const TArray<int32>& values, float height = 0.67f, float shift = 0.f, int offset = 0)
	{
		ImPlot::PlotBarsH(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), height, shift, offset, sizeof(int32));
	}

	// Plots a horizontal bar graph. #height and #shift are in Y units.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotBarsH_IntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, float height, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotBarsH(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), height, offset, sizeof(int32));
	}

	// Plots a horizontal bar graph. #height and #shift are in Y units.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotBarsH_FloatA(const FString& label_id, const TArray<float>& values, float height = 0.67f, float shift = 0.f, int offset = 0)
	{
		ImPlot::PlotBarsH<float>(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), height, shift, offset, sizeof(float));
	}

	// Plots a horizontal bar graph. #height and #shift are in Y units.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotBarsH_FloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, float height, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotBarsH<float>(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), height, offset, sizeof(float));	
	}
	

	// PlotErrorBars


	// Plots vertical error bar. The label_id should be the same as the label_id of the associated line or bar plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotErrorBarsIntA(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, const TArray<int32>& err, int offset = 0)
	{
		check(xs.Num() == ys.Num() && xs.Num() == err.Num());
		ImPlot::PlotErrorBars<int>(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), err.GetData(), xs.Num(), offset, sizeof(int32));
	}

	// Plots vertical error bar. The label_id should be the same as the label_id of the associated line or bar plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotErrorBarsIntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, const TArray<int32>& neg, const TArray<int32>& pos, int offset = 0)
	{
		check(xs.Num() == ys.Num() && xs.Num() == neg.Num() && xs.Num() == pos.Num());
		ImPlot::PlotErrorBars<int>(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), neg.GetData(), ys.GetData(), pos.Num(), offset, sizeof(int32));
	}

	// Plots vertical error bar. The label_id should be the same as the label_id of the associated line or bar plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotErrorBarsFloatA(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, const TArray<float>& err, int offset = 0)
	{
		check(xs.Num() == ys.Num() && xs.Num() == err.Num());
		ImPlot::PlotErrorBars<float>(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), err.GetData(), xs.Num(), offset, sizeof(float));
	}

	// Plots vertical error bar. The label_id should be the same as the label_id of the associated line or bar plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotErrorBarsFloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, const TArray<float>& neg, const TArray<float>& pos, int offset = 0)
	{
		check(xs.Num() == ys.Num() && xs.Num() == neg.Num() && xs.Num() == pos.Num());
		ImPlot::PlotErrorBars<float>(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), neg.GetData(), pos.GetData(), xs.Num(), offset, sizeof(float));
	}

	
	// PlotErrorBarsH


	// Plots horizontal error bars. The label_id should be the same as the label_id of the associated line or bar plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotErrorBarsH_IntA(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, const TArray<int32>& err, int offset = 0)
	{
		check(xs.Num() == ys.Num() && xs.Num() == err.Num());
		ImPlot::PlotErrorBarsH(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), err.GetData(), xs.Num(), offset, sizeof(int32));
	}

	// Plots horizontal error bars. The label_id should be the same as the label_id of the associated line or bar plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotErrorBarsH_IntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, const TArray<int32>& neg, const TArray<int32>& pos, int offset = 0)
	{
		check(xs.Num() == ys.Num() && xs.Num() == neg.Num() && xs.Num() == pos.Num());
		ImPlot::PlotErrorBarsH(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), neg.GetData(), ys.GetData(), pos.Num(), offset, sizeof(int32));
	}

	// Plots horizontal error bars. The label_id should be the same as the label_id of the associated line or bar plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotErrorBarsH_FloatA(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, const TArray<float>& err, int offset = 0)
	{
		check(xs.Num() == ys.Num() && xs.Num() == err.Num());
		ImPlot::PlotErrorBarsH(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), err.GetData(), xs.Num(), offset, sizeof(float));
	}

	// Plots horizontal error bars. The label_id should be the same as the label_id of the associated line or bar plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotErrorBarsH_FloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, const TArray<float>& neg, const TArray<float>& pos, int offset = 0)
	{
		check(xs.Num() == ys.Num() && xs.Num() == neg.Num() && xs.Num() == pos.Num());
		ImPlot::PlotErrorBarsH(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), neg.GetData(), pos.GetData(), xs.Num(), offset, sizeof(float));
	}


	// PlotStems

	// Plots vertical stems.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotStemsIntA(const FString& label_id, const TArray<int32>& values, float y_ref = 0, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotStems(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), y_ref, xscale, x0, offset, sizeof(int32));
	}

	// Plots vertical stems.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotStemsIntB(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, float y_ref = 0, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotStems(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), y_ref, offset, sizeof(int32));	
	}

	// Plots vertical stems.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotStemsFloatA(const FString& label_id, const TArray<float>& values, float y_ref = 0, float xscale = 1.0f, float x0 = 0.0f, int offset = 0)
	{
		ImPlot::PlotStems(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), y_ref, xscale, x0, offset, sizeof(float));
	}

	// Plots vertical stems.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotStemsFloatB(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, float y_ref = 0, int offset = 0)
	{
		check(xs.Num() == ys.Num());
		ImPlot::PlotStems(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), y_ref, offset, sizeof(float));	
	}

	
	// PlotVLines, PlotHLines

	
	/// Plots infinite vertical or horizontal lines (e.g. for references or asymptotes).
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotVLinesInt(const FString& label_id, const TArray<int32>& xs, int offset = 0)
	{
		ImPlot::PlotVLines(TCHAR_TO_ANSI(*label_id), xs.GetData(), xs.Num(), offset, sizeof(int32));
	}

	/// Plots infinite vertical or horizontal lines (e.g. for references or asymptotes).
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotVLinesFloat(const FString& label_id, const TArray<float>& xs, int offset = 0)
	{
		ImPlot::PlotVLines(TCHAR_TO_ANSI(*label_id), xs.GetData(), xs.Num(), offset, sizeof(float));
	}

	/// Plots infinite vertical or horizontal lines (e.g. for references or asymptotes).
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotHLinesInt(const FString& label_id, const TArray<int32>& xs, int offset = 0)
	{
		ImPlot::PlotHLines(TCHAR_TO_ANSI(*label_id), xs.GetData(), xs.Num(), offset, sizeof(int32));
	}

	/// Plots infinite vertical or horizontal lines (e.g. for references or asymptotes).
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotHLinesFloat(const FString& label_id, const TArray<float>& xs, int offset = 0)
	{
		ImPlot::PlotHLines(TCHAR_TO_ANSI(*label_id), xs.GetData(), xs.Num(), offset, sizeof(float));
	}

	
	// Plots a pie chart. If the sum of values > 1 or normalize is true, each value will be normalized. Center and radius are in plot units. #label_fmt can be set to NULL for no labels.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotPieChartInt(const TArray<FString>& label_ids, const TArray<int32>& values, float x, float y, float radius, bool normalize = false, FString label_fmt = TEXT("%.1f"), float angle0 = 90.0f)
	{
		ConvertArrayFStringToArrayAnsi labelList(label_ids);
		ImPlot::PlotPieChart(labelList.GetData(), values.GetData(), values.Num(), x, y, radius, normalize, TCHAR_TO_ANSI(*label_fmt), angle0);
	}

	// Plots a pie chart. If the sum of values > 1 or normalize is true, each value will be normalized. Center and radius are in plot units. #label_fmt can be set to NULL for no labels.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotPieChartFloat(const TArray<FString>& label_ids, const TArray<float>& values, float x, float y, float radius, bool normalize = false, FString label_fmt = TEXT("%.1f"), float angle0 = 90.0f)
	{
		ConvertArrayFStringToArrayAnsi labelList(label_ids);
		ImPlot::PlotPieChart(labelList.GetData(), values.GetData(), values.Num(), x, y, radius, normalize, TCHAR_TO_ANSI(*label_fmt), angle0);
	}
	
	// Plots a 2D heatmap chart. Values are expected to be in row-major order. Leave #scale_min and scale_max both at 0 for automatic color scaling, or set them to a predefined range. #label_fmt can be set to NULL for no labels.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AutoCreateRefTerm = "bounds_min, bounds_max"))
	static void PlotHeatmapInt(const FString& label_id, const TArray<int32>& values, int rows, int cols, float scale_min = 0.0f, float scale_max = 0, FString label_fmt = TEXT("%.1f"), float bounds_min_x = 0.0f, float bounds_min_y = 0.0f, float bounds_max_x = 0.0f, float bounds_max_y = 0.0f)
	{
		ImPlot::PlotHeatmap(TCHAR_TO_ANSI(*label_id), values.GetData(), rows, cols, scale_min, scale_max, TCHAR_TO_ANSI(*label_fmt), ImPlotPoint(bounds_min_x, bounds_min_y), ImPlotPoint(bounds_max_x, bounds_max_y));
	}
	
	// Plots a 2D heatmap chart. Values are expected to be in row-major order. Leave #scale_min and scale_max both at 0 for automatic color scaling, or set them to a predefined range. #label_fmt can be set to NULL for no labels.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", meta = (AutoCreateRefTerm = "bounds_min, bounds_max"))
	static void PlotHeatmapFloat(const FString& label_id, const TArray<float>& values, int rows, int cols, float scale_min = 0.0f, float scale_max = 0, FString label_fmt = TEXT("%.1f"), float bounds_min_x = 0.0f, float bounds_min_y = 0.0f, float bounds_max_x = 1.0f, float bounds_max_y = 1.0f)
	{
		ImPlot::PlotHeatmap<float>(TCHAR_TO_ANSI(*label_id), values.GetData(), rows, cols, scale_min, scale_max, label_fmt.Len() > 0 ? TCHAR_TO_ANSI(*label_fmt) : nullptr, ImPlotPoint(bounds_min_x, bounds_min_y), ImPlotPoint(bounds_max_x, bounds_max_y));
	}

	// Plots a horizontal histogram. #bins can be a positive integer or an ImPlotBin_ method. If #cumulative is true, each bin contains its count plus the counts of all previous bins.
	// If #density is true, the PDF is visualized. If both are true, the CDF is visualized. If #range is left unspecified, the min/max of #values will be used as the range.
	// If #range is specified, outlier values outside of the range are not binned. However, outliers still count toward normalizing and cumulative counts unless #outliers is false. The largest bin count or density is returned.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", Meta = (ReturnDisplayName = "Value"))
	static float PlotHistogramInt(const FString& label_id, const TArray<int32>& values, const TEnumAsByte<EImPlotBin::Type> bins = EImPlotBin::Type::ImPlotBin_Sturges,
		bool cumulative = false, bool density = false, float rangeMin = 0, float rangeMax = 0, bool outliers = true, float bar_scale = 1.0f)
	{
		return ImPlot::PlotHistogram(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), bins, cumulative, density, ImPlotRange(rangeMin, rangeMax), outliers, bar_scale);
	}

	UFUNCTION(BlueprintCallable, Category = "Implot|Item", Meta = (ReturnDisplayName = "Value"))
	static float PlotHistogramFloat(const FString& label_id, const TArray<float>& values, TEnumAsByte<EImPlotBin::Type> bins = EImPlotBin::Type::ImPlotBin_Sturges,
		bool cumulative = false, bool density = false, float rangeMin = 0, float rangeMax = 0, bool outliers = true, float bar_scale = 1.0f)
	{
		return ImPlot::PlotHistogram(TCHAR_TO_ANSI(*label_id), values.GetData(), values.Num(), bins, cumulative, density, ImPlotRange(rangeMin, rangeMax), outliers, bar_scale);
	}

	// Plots two dimensional, bivariate histogram as a heatmap. #x_bins and #y_bins can be a positive integer or an ImPlotBin. If #density is true, the PDF is visualized.
	// If #range is left unspecified, the min/max of #xs an #ys will be used as the ranges. If #range is specified, outlier values outside of range are not binned.
	// However, outliers still count toward the normalizing count for density plots unless #outliers is false. The largest bin count or density is returned.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item", Meta = (ReturnDisplayName = "Value"))
	static float PlotHistogram2DInt(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, FVector2D rangeMin, FVector2D rangeMax, TEnumAsByte<EImPlotBin::Type> x_bins = EImPlotBin::Type::ImPlotBin_Sturges, TEnumAsByte<EImPlotBin::Type> y_bins = EImPlotBin::Type::ImPlotBin_Sturges, bool density=false, bool outliers=true)
	{
		check(xs.Num() == ys.Num())
		return ImPlot::PlotHistogram2D(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), x_bins, y_bins, density, ImPlotLimits(rangeMin.X, rangeMax.X, rangeMin.Y, rangeMax.Y), outliers);
	}

	UFUNCTION(BlueprintCallable, Category = "Implot|Item", Meta = (ReturnDisplayName = "Value"))
	static float PlotHistogram2DFloat(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, FVector2D rangeMin, FVector2D rangeMax, TEnumAsByte<EImPlotBin::Type> x_bins = EImPlotBin::Type::ImPlotBin_Sturges, TEnumAsByte<EImPlotBin::Type> y_bins = EImPlotBin::Type::ImPlotBin_Sturges, bool density=false, bool outliers=true)
	{
		check(xs.Num() == ys.Num())
		return ImPlot::PlotHistogram2D(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), x_bins, y_bins, density, ImPlotLimits(rangeMin.X, rangeMax.X, rangeMin.Y, rangeMax.Y), outliers);
	}
	
	
	// Plots digital data. Digital plots do not respond to y drag or zoom, and are always referenced to the bottom of the plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotDigitalInt(const FString& label_id, const TArray<int32>& xs, const TArray<int32>& ys, int offset = 0)
	{
		check(xs.Num() == ys.Num())
		ImPlot::PlotDigital(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), offset, sizeof(int32));
	}

	// Plots digital data. Digital plots do not respond to y drag or zoom, and are always referenced to the bottom of the plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotDigitalFloat(const FString& label_id, const TArray<float>& xs, const TArray<float>& ys, int offset = 0)
	{
		check(xs.Num() == ys.Num())
		ImPlot::PlotDigital(TCHAR_TO_ANSI(*label_id), xs.GetData(), ys.GetData(), xs.Num(), offset, sizeof(float));
	}
	
	// UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	// static void PlotDigitalG(const char* label_id, ImPlotPoint (*getter)(void* data, int idx), void* data, int count, int offset=0)
	

	
	//UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotImage(const FString& label_id, ImTextureID user_texture_id, const FImPlotPoint& bounds_min, const FImPlotPoint& bounds_max, const FVector2D& uv0, const FVector2D& uv1, const FVector4& tint_col)
	{ ImPlot::PlotImage(TCHAR_TO_ANSI(*label_id), user_texture_id, bounds_min, bounds_max, ToImVec2(uv0), ToImVec2(uv1), ToImVec4(tint_col));}
	
	// Plots a centered text label at point x,y with optional pixel offset. Text color can be changed with ImPlot::PushStyleColor(ImPlotCol_InlayText, ...).
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotText(const FString& text, float x, float y, bool vertical = false, FVector2D pix_offset = FVector2D(0,0))
	{ ImPlot::PlotText(TCHAR_TO_ANSI(*text), x, y, vertical, ToImVec2(pix_offset));}
 
	// Plots an dummy item (i.e. adds a legend entry colored by ImPlotCol_Line)
	UFUNCTION(BlueprintCallable, Category = "Implot|Item")
	static void PlotDummy(const FString& label_id) { ImPlot::PlotDummy(TCHAR_TO_ANSI(*label_id));}
 
	//-----------------------------------------------------------------------------
	// Plot Utils
	//-----------------------------------------------------------------------------
  
	// Set the axes range limits of the next plot. Call right before BeginPlot(). If ImGuiCond_Always is used, the axes limits will be locked.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils", meta = (AdvancedDisplay = 4))
	static void SetNextPlotLimits(float xmin, float xmax, float ymin, float ymax, int32 ImGuiCond = 2/*ImGuiCond_Once*/)
	{
		ImPlot::SetNextPlotLimits(xmin, xmax, ymin, ymax, ImGuiCond);
	}
	
	// Set the X axis range limits of the next plot. Call right before BeginPlot(). If ImGuiCond_Always is used, the X axis limits will be locked.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void SetNextPlotLimitsX(float xmin, float xmax, int32 ImGuiCond = 2/*ImGuiCond_Once*/)
	{ ImPlot::SetNextPlotLimitsX(xmin, xmax, ImGuiCond); }

	// Set the Y axis range limits of the next plot. Call right before BeginPlot(). If ImGuiCond_Always is used, the Y axis limits will be locked.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void SetNextPlotLimitsY(float ymin, float ymax, int32 ImGuiCond = 2/*ImGuiCond_Once*/, EImPlotYAxis y_axis = EImPlotYAxis::ImPlotYAxis_1)
	{ ImPlot::SetNextPlotLimitsY(ymin, ymax, ImGuiCond, static_cast<ImPlotYAxis>(y_axis)); }
	
	// Links the next plot limits to external values. Set to NULL for no linkage. The pointer data must remain valid until the matching call EndPlot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void LinkNextPlotLimits(UPARAM(ref) float& xmin, UPARAM(ref) float& xmax, UPARAM(ref) float& ymin, UPARAM(ref) float& ymax)
	{
		double local_xmin = xmin;
		double local_xmax = xmax;
		double local_ymin = ymin;
		double local_ymax = ymax;
		ImPlot::LinkNextPlotLimits(&local_xmin, &local_xmax, &local_ymin, &local_ymax, nullptr, nullptr, nullptr, nullptr);
		xmin = local_xmin;
		xmax = local_xmax;
		ymin = local_ymin;
		ymax = local_ymax;
	}
	
	// Fits the next plot axes to all plotted data if they are unlocked (equivalent to double-clicks).
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils", meta = (AdvancedDisplay = "x,y,y2,y3"))
	static void FitNextPlotAxes(bool x = true, bool y = true, bool y2 = true, bool y3 = true)
	{ ImPlot::FitNextPlotAxes(x, y, y2, y3); }
 
	// Set the X axis ticks and optionally the labels for the next plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void SetNextPlotTicksX(UPARAM(ref) TArray<float>& values, UPARAM(ref) TArray<FString>& labels, bool show_default = false)
	{
		ConvertArrayFStringToArrayAnsi labelList(labels);
		ImPlot::SetNextPlotTicksX((double*)values.GetData(), labels.Num(), labelList.GetData(), show_default);
	}

	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void SetNextPlotTicksX_A(float x_min, float x_max, UPARAM(ref) TArray<FString>& labels, bool show_default = false)
	{
		ConvertArrayFStringToArrayAnsi labelList(labels);
		ImPlot::SetNextPlotTicksX(x_min, x_max, labels.Num(), labelList.GetData(), show_default);
	}
	// static void SetNextPlotTicksX(double x_min, double x_max, int n_ticks, const char* const labels[] = NULL, bool show_default = false);
	//

	// Set the Y axis ticks and optionally the labels for the next plot.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void SetNextPlotTicksY(UPARAM(ref) TArray<float>& values, UPARAM(ref) TArray<FString>& labels, bool show_default = false, EImPlotYAxis y_axis = EImPlotYAxis::ImPlotYAxis_1)
	{
		ConvertArrayFStringToArrayAnsi labelList(labels);
		ImPlot::SetNextPlotTicksY((double*)values.GetData(), labels.Num(), labelList.GetData(), show_default, static_cast<ImPlotYAxis>(y_axis));
	}
	
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void SetNextPlotTicksY_A(float y_min, float y_max, UPARAM(ref) TArray<FString>& labels, bool show_default = false, EImPlotYAxis y_axis = EImPlotYAxis::ImPlotYAxis_1)
	{
		ConvertArrayFStringToArrayAnsi labelList(labels);
		ImPlot::SetNextPlotTicksY(y_min, y_max, labels.Num(), labelList.GetData(), show_default, static_cast<ImPlotYAxis>(y_axis));
	}
	
	
	// The following functions MUST be called BETWEEN Begin/EndPlot!
 
	// Select which Y axis will be used for subsequent plot elements. The default is ImPlotYAxis_1, or the first (left) Y axis. Enable 2nd and 3rd axes with ImPlotFlags_YAxisX.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void SetPlotYAxis(int32 ImPlotYAxis_y_axis)
	{ ImPlot::SetPlotYAxis(ImPlotYAxis_y_axis); }

	// Hides or shows the next plot item (i.e. as if it were toggled from the legend). Use ImGuiCond_Always if you need to forcefully set this every frame.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static void HideNextItem(bool hidden = true, int32 ImGuiCond = 2)
	{ ImPlot::HideNextItem(hidden, ImGuiCond); }
 
	// Convert pixels to a position in the current plot's coordinate system. A negative y_axis uses the current value of SetPlotYAxis (ImPlotYAxis_1 initially).
	UFUNCTION(BlueprintPure, Category = "Implot|Utils", Meta = (ReturnDisplayName = "Position"))
	static FImPlotPoint PixelsToPlot(float x, float y, int32 ImPlotYAxis_y_axis = -1)
	{ return ImPlot::PixelsToPlot(x, y, ImPlotYAxis_y_axis); }

	// Convert a position in the current plot's coordinate system to pixels. A negative y_axis uses the current value of SetPlotYAxis (ImPlotYAxis_1 initially).
	UFUNCTION(BlueprintPure, Category = "Implot|Utils", Meta = (ReturnDisplayName = "Pixels"))
	static FVector2D PlotToPixels(float x, float y, int32 ImPlotYAxis_y_axis = -1)
	{ return ToVector2D(ImPlot::PlotToPixels(x, y, ImPlotYAxis_y_axis)); }

	// Get the current Plot position (top-left) in pixels.
	UFUNCTION(BlueprintPure, Category = "Implot|Utils", Meta = (ReturnDisplayName = "Position"))
	static FVector2D GetPlotPos()
	{ return ToVector2D(ImPlot::GetPlotPos()); }

	// Get the curent Plot size in pixels.
	UFUNCTION(BlueprintPure, Category = "Implot|Utils", Meta = (ReturnDisplayName = "Size"))
	static FVector2D GetPlotSize()
	{ return ToVector2D(ImPlot::GetPlotSize()); }

	// Returns true if the plot area in the current plot is hovered.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static bool IsPlotHovered()
	{ return ImPlot::IsPlotHovered(); }

	// Returns true if the XAxis plot area in the current plot is hovered.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static bool IsPlotXAxisHovered()
	{ return ImPlot::IsPlotXAxisHovered(); }
	
	// Returns true if the YAxis[n] plot area in the current plot is hovered.
	UFUNCTION(BlueprintCallable, Category = "Implot|Utils")
	static bool IsPlotYAxisHovered(int32 ImPlotYAxis_y_axis = 0)
	{ return ImPlot::IsPlotYAxisHovered(ImPlotYAxis_y_axis); }

	// Returns the mouse position in x,y coordinates of the current plot. A negative y_axis uses the current value of SetPlotYAxis (ImPlotYAxis_1 initially).
	UFUNCTION(BlueprintPure, Category = "Implot|Utils", Meta = (ReturnDisplayName = "MousePos"))
	static FImPlotPoint GetPlotMousePos(int32 ImPlotYAxis_y_axis = -1)
	{ return ImPlot::GetPlotMousePos(ImPlotYAxis_y_axis); }
	
	// Returns the current plot axis range. A negative y_axis uses the current value of SetPlotYAxis (ImPlotYAxis_1 initially).
	UFUNCTION(BlueprintPure, Category = "Implot|Utils", Meta = (ReturnDisplayName = "Limits"))
	static FImPlotLimits GetPlotLimits(int32 ImPlotYAxis_y_axis = -1)
	{ return ImPlot::GetPlotLimits(ImPlotYAxis_y_axis); }
 
	// Returns true if the current plot is being queried. Query must be enabled with ImPlotFlags_Query.
	UFUNCTION(BlueprintPure, Category = "Implot|Utils")
	static bool IsPlotQueried()
	{ return ImPlot::IsPlotQueried(); }
	
	// Returns the current plot query bounds. Query must be enabled with ImPlotFlags_Query.
	UFUNCTION(BlueprintPure, Category = "Implot|Utils", Meta = (ReturnDisplayName = "Limits"))
	static FImPlotLimits GetPlotQuery(int32 ImPlotYAxis_y_axis = -1)
	{ return ImPlot::GetPlotQuery(ImPlotYAxis_y_axis); }
 
	//-----------------------------------------------------------------------------
	// Plot Tools
	//-----------------------------------------------------------------------------
 
	// The following functions MUST be called BETWEEN Begin/EndPlot!
 
	// Shows an annotation callout at a chosen point.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot Tools")
	static void Annotate(float x, float y, const FVector2D& pix_offset, FLinearColor color, const FString& fmt)
	{ ImPlot::Annotate(x, y, ToImVec2(pix_offset), ToImVec4(color), TCHAR_TO_ANSI(*fmt), nullptr); }

	// Same as above, but the annotation will always be clamped to stay inside the plot area.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot Tools")
	static void AnnotateClamped(float x, float y, const FVector2D& pix_offset, FLinearColor color, const FString& fmt)
	{ ImPlot::AnnotateClamped(x, y, ToImVec2(pix_offset), ToImVec4(color), TCHAR_TO_ANSI(*fmt), nullptr); }
 
	// Shows a draggable vertical guide line at an x-value. #col defaults to ImGuiCol_Text.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot Tools")
	static bool DragLineX(const FString& id, UPARAM(ref) float& x_value, bool show_label = true, FLinearColor color = FLinearColor(0, 0, 0, -1), float thickness = 1)
	{
		double x = x_value;
		bool succeeded = ImPlot::DragLineX(TCHAR_TO_ANSI(*id), &x, show_label, ToImVec4(color), thickness);
		x_value = x;
		return succeeded;
	}
	
	// Shows a draggable horizontal guide line at a y-value. #col defaults to ImGuiCol_Text.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot Tools")
	static bool DragLineY(const FString& id, UPARAM(ref) float& y_value, bool show_label = true, FLinearColor color = FLinearColor(0, 0, 0, -1), float thickness = 1)
	{
		double y = y_value;
		bool succeeded = ImPlot::DragLineY(TCHAR_TO_ANSI(*id), &y, show_label, ToImVec4(color), thickness);
		y_value = y;
		return succeeded;
	}
	
	// Shows a draggable point at x,y. #col defaults to ImGuiCol_Text.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot Tools")
	static bool DragPoint(const FString& id, UPARAM(ref) float& x, UPARAM(ref) float& y, bool show_label = true, FLinearColor color = FLinearColor(0, 0, 0, -1), float radius = 4)
	{
		double inX = x; double inY = y;
		bool succeeded = ImPlot::DragPoint(TCHAR_TO_ANSI(*id), &inX, &inY, show_label, ToImVec4(color), radius);
		x = inX; y = inY;
		return succeeded;
	}
 
	//-----------------------------------------------------------------------------
	// Legend Utils and Tools
	//-----------------------------------------------------------------------------
 
	// The following functions MUST be called BETWEEN Begin/EndPlot!
 
	// Set the location of the current plot's legend.
	UFUNCTION(BlueprintCallable, Category = "Implot|Legend Utils and Tools")
	static void SetLegendLocation(TEnumAsByte<EImPlotLocation::Type> location, EImPlotOrientation orientation = EImPlotOrientation::ImPlotOrientation_Vertical, bool outside = false)
	{ ImPlot::SetLegendLocation(location, static_cast<ImPlotOrientation>(orientation), outside); }

	// Set the location of the current plot's mouse position text (default = South|East).
	UFUNCTION(BlueprintCallable, Category = "Implot|Legend Utils and Tools")
	static void SetMousePosLocation(TEnumAsByte<EImPlotLocation::Type> location)
	{ ImPlot::SetMousePosLocation(location); }

	// Returns true if a plot item legend entry is hovered.
	UFUNCTION(BlueprintCallable, Category = "Implot|Legend Utils and Tools")
	static bool IsLegendEntryHovered(const FString& label_id)
	{ return ImPlot::IsLegendEntryHovered(TCHAR_TO_ANSI(*label_id)); }
 
	// Begin a popup for a legend entry.
	UFUNCTION(BlueprintCallable, Category = "Implot|Legend Utils and Tools")
	static bool BeginLegendPopup(const FString& label_id, int32 ImGuiMouseButton = 1)
	{ return ImPlot::BeginLegendPopup(TCHAR_TO_ANSI(*label_id), ImGuiMouseButton); }

	// End a popup for a legend entry.
	UFUNCTION(BlueprintCallable, Category = "Implot|Legend Utils and Tools")
	static void EndLegendPopup()
	{ ImPlot::EndLegendPopup(); }
 
	//-----------------------------------------------------------------------------
	// Drag and Drop Utils
	//-----------------------------------------------------------------------------
 
	// Turns the current plot's plotting area into a drag and drop target. Don't forget to call EndDragDropTarget!
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static bool BeginDragDropTarget()
	{ return ImPlot::BeginDragDropTarget(); }

	// Turns the current plot's X-axis into a drag and drop target. Don't forget to call EndDragDropTarget!
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static bool BeginDragDropTargetX()
	{ return ImPlot::BeginDragDropTargetX(); }
	
	// Turns the current plot's Y-Axis into a drag and drop target. Don't forget to call EndDragDropTarget!
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static bool BeginDragDropTargetY(EImPlotYAxis axis = EImPlotYAxis::ImPlotYAxis_1)
	{ return ImPlot::BeginDragDropTargetY(static_cast<ImPlotYAxis>(axis)); }

	// Turns the current plot's legend into a drag and drop target. Don't forget to call EndDragDropTarget!
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static bool BeginDragDropTargetLegend()
	{ return ImPlot::BeginDragDropTargetLegend(); }

	// Ends a drag and drop target (currently just an alias for ImGui::EndDragDropTarget).
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static void EndDragDropTarget()
	{ ImPlot::EndDragDropTarget(); }
	
 
	// NB: By default, plot and axes drag and drop sources require holding the Ctrl modifier to initiate the drag.
	// You can change the modifier if desired. If ImGuiKeyModFlags_None is provided, the axes will be locked from panning.
 
	// Turns the current plot's plotting area into a drag and drop source. Don't forget to call EndDragDropSource!
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static bool BeginDragDropSource(int32 ImGuiKeyModFlags_key_mode = 1, int32 ImGuiDragDropFlags = 0)
	{ return ImPlot::BeginDragDropSource(ImGuiKeyModFlags_key_mode, ImGuiDragDropFlags); }

	// Turns the current plot's X-axis into a drag and drop source. Don't forget to call EndDragDropSource!
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static bool BeginDragDropSourceX(int32 ImGuiKeyModFlags_key_mode = 1, int32 ImGuiDragDropFlags = 0)
	{ return ImPlot::BeginDragDropSourceX(ImGuiKeyModFlags_key_mode, ImGuiDragDropFlags); }

	// Turns the current plot's Y-axis into a drag and drop source. Don't forget to call EndDragDropSource!
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static bool BeginDragDropSourceY(EImPlotYAxis axis = EImPlotYAxis::ImPlotYAxis_1, int32 ImGuiKeyModFlags_key_mode = 1, int32 ImGuiDragDropFlags = 0)
	{ return ImPlot::BeginDragDropSourceY(static_cast<ImPlotYAxis>(axis), ImGuiKeyModFlags_key_mode, ImGuiDragDropFlags); }

	// Turns an item in the current plot's legend into drag and drop source. Don't forget to call EndDragDropSource!
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static bool BeginDragDropSourceItem(const FString& label_id, int32 ImGuiDragDropFlags = 0)
	{ return ImPlot::BeginDragDropSourceItem(TCHAR_TO_ANSI(*label_id), ImGuiDragDropFlags); }
	
	// Ends a drag and drop source (currently just an alias for ImGui::EndDragDropSource).
	UFUNCTION(BlueprintCallable, Category = "Implot|Drag and Drop Utils")
	static void EndDragDropSource()
	{ ImPlot::EndDragDropSource(); }
 
	//-----------------------------------------------------------------------------
	// Plot and Item Styling
	//-----------------------------------------------------------------------------
 
	// Provides access to plot style structure for permanant modifications to colors, sizes, etc.
	//UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static ImPlotStyle& GetStyle() { return ImPlot::GetStyle(); }
 
	// Style colors for current ImGui style (default).
	//UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void StyleColorsAuto(ImPlotStyle* dst = NULL) { ImPlot::StyleColorsAuto(dst); }

	// Style colors for ImGui "Classic".
	//UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void StyleColorsClassic(ImPlotStyle* dst = NULL){ ImPlot::StyleColorsClassic(dst); }

	// Style colors for ImGui "Dark".
	//UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void StyleColorsDark(ImPlotStyle* dst = NULL){ ImPlot::StyleColorsDark(dst); }

	// Style colors for ImGui "Light".
	//UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void StyleColorsLight(ImPlotStyle* dst = NULL) { ImPlot::StyleColorsLight(dst); }
 
	// Use PushStyleX to temporarily modify your ImPlotStyle. The modification
	// will last until the matching call to PopStyleX. You MUST call a pop for
	// every push, otherwise you will leak memory! This behaves just like ImGui.
 
	// Temporarily modify a plot color. Don't forget to call PopStyleColor!
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void PushStyleColor(EImPlotCol idx, const FLinearColor& color)
	{
		ImVec4 col = ToImVec4(color);
		ImPlot::PushStyleColor(static_cast<ImPlotCol>(idx), col);
	}

	
	// Undo temporary color modification. Undo multiple pushes at once by increasing count.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void PopStyleColor(int32 count = 1)
	{ ImPlot::PopStyleColor(count); }
 
	// Temporarily modify a style variable of float type. Don't forget to call PopStyleVar!
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void PushStyleVar(EImPlotStyleVar idx, float val)
	{ ImPlot::PushStyleVar(static_cast<ImPlotStyleVar>(idx), val); }

	// Temporarily modify a style variable of int type. Don't forget to call PopStyleVar!
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void PushStyleVarA(EImPlotStyleVar idx, int val)
	{ ImPlot::PushStyleVar(static_cast<ImPlotStyleVar>(idx), val); }

	// Temporarily modify a style variable of ImVec2 type. Don't forget to call PopStyleVar!
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void PushStyleVarB(EImPlotStyleVar idx, const FVector2D& val)
	{ ImPlot::PushStyleVar(static_cast<ImPlotStyleVar>(idx), ToImVec2(val)); }

	// Undo temporary style modification. Undo multiple pushes at once by increasing count.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling", meta=(AdvancedDisplay = "count"))
	static void PopStyleVar(int32 count = 1)
	{ ImPlot::PopStyleVar(count); }
 
	// The following can be used to modify the style of the next plot item ONLY. They do
	// NOT require calls to PopStyleX. Leave style attributes you don't want modified to
	// IMPLOT_AUTO or IMPLOT_AUTO_COL. Automatic styles will be deduced from the current
	// values in your ImPlotStyle or from Colormap data.
 
	// Set the line color and weight for the next item only.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void SetNextLineStyle(FLinearColor col = FLinearColor(0, 0, 0, -1), float weight = -1)
	{ ImPlot::SetNextLineStyle(ToImVec4(col), weight); }
	
	// Set the fill color for the next item only.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void SetNextFillStyle(FLinearColor col = FLinearColor(0, 0, 0, -1), float alpha_mod = -1)
	{ ImPlot::SetNextFillStyle(ToImVec4(col), alpha_mod); }

	// Set the marker style for the next item only.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling", meta = (AdvancedDisplay = 1))
	static void SetNextMarkerStyle(const TEnumAsByte<EImPlotMarker::Type> marker = EImPlotMarker::Type::ImPlotMarker_None,
		float size = -1, FLinearColor fill = FLinearColor(0, 0, 0, -1), float weight = -1, FLinearColor outline = FLinearColor(0, 0, 0, -1))
	{ ImPlot::SetNextMarkerStyle(marker, size, ToImVec4(fill), weight, ToImVec4(outline)); }

	// Set the error bar style for the next item only.
	UFUNCTION(BlueprintCallable, Category = "Implot|Plot and Item Styling")
	static void SetNextErrorBarStyle(FLinearColor col = FLinearColor(0, 0, 0, -1), float size = -1, float weight = -1)
	{ ImPlot::SetNextErrorBarStyle(ToImVec4(col), size, weight); }
 
	// Gets the last item primary color (i.e. its legend icon color)
	UFUNCTION(BlueprintPure, Category = "Implot|Plot and Item Styling", Meta = (ReturnDisplayName = "PrimaryColor"))
	static FLinearColor GetLastItemColor()
	{ return ToLinearColor(ImPlot::GetLastItemColor()); }
 
	// Returns the null terminated string name for an ImPlotCol.
	UFUNCTION(BlueprintPure, Category = "Implot|Plot and Item Styling", Meta = (ReturnDisplayName = "ColorName"))
	static FString GetStyleColorName(int32 ImPlotCol)
	{ return ANSI_TO_TCHAR(ImPlot::GetStyleColorName(ImPlotCol)); }
	
	// Returns the null terminated string name for an ImPlotMarker.
	UFUNCTION(BlueprintPure, Category = "Implot|Plot and Item Styling", Meta = (ReturnDisplayName = "MarkerName"))
	static FString GetMarkerName(TEnumAsByte<EImPlotMarker::Type> idx)
	{ return ANSI_TO_TCHAR(ImPlot::GetMarkerName(idx)); }
	
	//-----------------------------------------------------------------------------
	// Colormaps
	//-----------------------------------------------------------------------------

	// Add a new colormap. The colormap can be used by pushing either the returned index or the string name with PushColormap.
	// The colormap name must be unique and the size must be greater than 1. You will receive an assert otherwise! By default
	// colormaps are considered to be qualitative (i.e. discrete). If you want to create a continuous colormap, set #qual=false.
	// This will treat the colors you provide as keys, and ImPlot will build a linearly interpolated lookup table that fills
	// in the gaps. The memory footprint of this table will be exactly ((size-1)*255+1)*4 bytes.
	// static EImPlotColormap AddColormap(const char* name, const ImVec4* cols, int size, bool qual=true);
	// static EImPlotColormap AddColormap(const char* name, const ImU32*  cols, int size, bool qual=true);
	
	// Returns the number of available colormaps.
	UFUNCTION(BlueprintPure, Category = "Implot|Colormaps", Meta = (ReturnDisplayName = "Count"))
	static int32 GetColormapCount()
	{ return ImPlot::GetColormapCount(); }
	
	// Returns a null terminated string name for a built-in colormap.
	UFUNCTION(BlueprintPure, Category = "Implot|Colormaps", Meta = (ReturnDisplayName = "Name"))
	static FString GetColormapName(int32 ImPlotColormap)
	{ return ANSI_TO_TCHAR(ImPlot::GetColormapName(ImPlotColormap)); }
	
	// Temporarily switch to one of the built-in colormaps.
	UFUNCTION(BlueprintCallable, Category = "Implot|Colormaps")
	static void PushColormap(EImPlotColormap cmap)
	{ ImPlot::PushColormap(static_cast<ImPlotColormap>(cmap)); }
	
	// Temporarily switch to your custom colormap. The pointer data must persist until the matching call to PopColormap!
	UFUNCTION(BlueprintCallable, Category = "Implot|Colormaps")
	static void PushColormapA(const FString& colorName)
	{
		ImPlot::PushColormap(TCHAR_TO_ANSI(*colorName));
	}
	
	// Undo temporary colormap modification.
	UFUNCTION(BlueprintCallable, Category = "Implot|Colormaps")
	static void PopColormap(int32 count = 1)
	{ ImPlot::PopColormap(count); }

	// Returns the next color from the current colormap and advances the colormap for the current plot.
	// Can also be used with no return value to skip colors if desired. You need to call this between Begin/EndPlot!
	UFUNCTION(BlueprintCallable, Category = "Implot|Colormaps")
	static FLinearColor NextColormapColor()
	{ return ToLinearColor(ImPlot::NextColormapColor()); }


	// Colormap utils. If cmap = IMPLOT_AUTO (default), the current colormap is assumed.
	// Pass an explicit colormap index (built-in or user added) to specify otherwise.

	
	// Returns the size of the current colormap.
	UFUNCTION(BlueprintPure, Category = "Implot|Colormaps", Meta = (ReturnDisplayName = "Size"))
	static int32 GetColormapSize()
	{ return ImPlot::GetColormapSize(); }
	
	// Returns a color from the Color map given an index >= 0 (modulo will be performed).
	UFUNCTION(BlueprintPure, Category = "Implot|Colormaps")
	static FLinearColor GetColormapColor(int32 index)
	{ return ToLinearColor(ImPlot::GetColormapColor(index)); }


	// Sample a color from the current colormap given t between 0 and 1.
	UFUNCTION(BlueprintPure, Category = "Implot|Colormaps")
	static FLinearColor SampleColormap(float t, EImPlotColormap cmap)
	{ return ToLinearColor(ImPlot::SampleColormap(t, static_cast<ImPlotColormap>(cmap))); }

	 
	// Shows a vertical color scale with linear spaced ticks using the specified color map. Use double hashes to hide label (e.g. "##NoLabel").
	UFUNCTION(BlueprintCallable, Category = "Implot|Colormaps")
	static void ColormapScale(const FString& label, float scale_min, float scale_max, FVector2D size, EImPlotColormap cmap)
	{ ImPlot::ColormapScale(TCHAR_TO_ANSI(*label), scale_min, scale_max, ToImVec2(size), static_cast<ImPlotColormap>(cmap)); }

	// Shows a horizontal slider with a colormap gradient background. Optionally returns the color sampled at t in [0 1].
	UFUNCTION(BlueprintCallable, Category = "Implot|Colormaps")
	static bool ColormapSlider(const FString& label, UPARAM(ref) float& t, UPARAM(ref) FVector4& out, FString format, EImPlotColormap cmap)
	{ return ImPlot::ColormapSlider(TCHAR_TO_ANSI(*label), &t, reinterpret_cast<ImVec4*>(&out), TCHAR_TO_ANSI(*format), static_cast<ImPlotColormap>(cmap)); }

	// Shows a button with a colormap gradient brackground.
	UFUNCTION(BlueprintCallable, Category = "Implot|Colormaps", meta = (ExpandEnumAsExecs="OutResult"))
	static void ColormapButton(const FString& label, FVector2D size, EImPlotColormap cmap, TEnumAsByte<EImGuiFlowControl::Type>& OutResult)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImPlot::ColormapButton(TCHAR_TO_ANSI(*label), ToImVec2(size), static_cast<ImPlotColormap>(cmap)))
			OutResult = EImGuiFlowControl::Success;
	}


	// When items in a plot sample their color from a colormap, the color is cached and does not change
	// unless explicitly overriden. Therefore, if you change the colormap after the item has already been plotted,
	// item colors will not update. If you need item colors to resample the new colormap, then use this
	// function to bust the cached colors. If #plot_title_id is NULL, then every item in EVERY existing plot
	// will be cache busted. Otherwise only the plot specified by #plot_title_id will be busted. For the
	// latter, this function must be called in the ImGui window that the plot is in. You should rarely if ever
	// need this function, but it is available for applications that require runtime swaps (see Heatmaps demo).
	UFUNCTION(BlueprintCallable, Category = "Implot|Colormaps")
	static void BustColorCache(FString plot_title_id)
	{ ImPlot::BustColorCache(plot_title_id.Len() > 0 ? TCHAR_TO_ANSI(*plot_title_id) : nullptr);}
	
 
	//-----------------------------------------------------------------------------
	// Miscellaneous
	//-----------------------------------------------------------------------------
 
	// Render a icon similar to those that appear in legends (nifty for data lists).
	UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous")
	static void ItemIcon(const FVector4& col)
	{ ImPlot::ItemIcon(ToImVec4(col)); }
	
	UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous")
	static void ItemIconA(int32 col)
	{ ImPlot::ItemIcon(col); }
 
	// Get the plot draw list for rendering to the current plot area.
	//UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous")
	static ImDrawList* GetPlotDrawList()
	{ return ImPlot::GetPlotDrawList(); }
	
	// Push clip rect for rendering to current plot area.
	UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous")
	static void PushPlotClipRect()
	{ ImPlot::PushPlotClipRect(); }
	
	// Pop plot clip rect.
	UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous")
	static void PopPlotClipRect()
	{ ImPlot::PopPlotClipRect(); }
 
	// Shows ImPlot style selector dropdown menu.
	UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous", meta = (ExpandEnumAsExecs="OutResult"))
	static void ShowStyleSelector(const FString& label, TEnumAsByte<EImGuiFlowControl::Type>& OutResult)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImPlot::ShowStyleSelector(TCHAR_TO_ANSI(*label)))
			OutResult = EImGuiFlowControl::Success;
	}
	
	// Shows ImPlot colormap selector dropdown menu.
	UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous", meta = (ExpandEnumAsExecs="OutResult"))
	static void ShowColormapSelector(const FString& label, TEnumAsByte<EImGuiFlowControl::Type>& OutResult)
	{
		OutResult = EImGuiFlowControl::Failure;
		if(ImPlot::ShowColormapSelector(TCHAR_TO_ANSI(*label)))
			OutResult = EImGuiFlowControl::Success;		
	}
	
	// Shows ImPlot style editor block (not a window).
	//UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous")
	static void ShowStyleEditor(ImPlotStyle* ref = NULL)
	{ ImPlot::ShowStyleEditor(ref); }
	
	// Add basic help/info block (not a window): how to manipulate ImPlot as an end-user.
	UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous")
	static void ShowUserGuide() { ImPlot::ShowUserGuide(); }
	
	// Shows ImPlot metrics/debug information.
	UFUNCTION(BlueprintCallable, Category = "Implot|Miscellaneous")
	static void ShowMetricsWindow(UPARAM(ref) bool& open)
	{
		if(open) { ImPlot::ShowMetricsWindow(&open); }
	}

	// Shows the ImPlot demo.
	UFUNCTION(BlueprintCallable, Category = "Implot|Demo")
	static void ShowDemoWindow() { ImPlot::ShowDemoWindow(); }
	
	
	UFUNCTION(BlueprintCallable, CustomThunk, meta=(DisplayName = "Shuffle2", CompactNodeTitle = "SHUFFLE", ArrayParm = "TargetArray"), Category="Utilities|Array")
	static void Array_Shuffle2(const TArray<int32>& TargetArray);
	
	DECLARE_FUNCTION(execArray_Shuffle2)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;
		P_NATIVE_BEGIN;
		//MARK_PROPERTY_DIRTY(Stack.Object, ArrayProperty);
		GenericArray_Shuffle2(ArrayAddr, ArrayProperty);
		P_NATIVE_END;
	}
	
	static void GenericArray_Shuffle2(void* TargetArray, const FArrayProperty* ArrayProp);
 
};
