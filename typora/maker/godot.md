```c#
// 反射
namespace Dan{
    [AttributeUsage(AttributeTargets.Class| AttributeTargets.Constructor| AttributeTargets.Field| AttributeTargets.Method, AllowMultiple= true)]
    public class DebugInfo: Attribute{
        public int BugNo{get;}
        public string Developer{get;}
        public string LastReview{get;}
        public string Message{get;set;}
        
        public DebugInfo(int bg, string dev, string d){ BugNo=bg; Developer=dev; LastReview=d; }
        
        public override string ToString()=> $"[{BugNo}] {Developer}: '{Message}' {LastReview}";
    }
    
    [DebugInfo(45, "Zara", "12/8/2012", Message="return type mismatch.")]
    [DebugInfo(49, "Nuha", "10/10/2012", Message="unused variable.")]
    public class Rectangle{
        public double Width{get; set;}
        public double Height{get; set}
        
        [DebugInfo(45, "Zara", "12/8/2012", Message="return type mismatch.")]
        public double Area{get{return Width* Height; }}
        
        [DebugInfo(45, "Zara", "12/8/2012")]
        public override string ToString()=> $"{Width}x{Height}";
    }
    
    //
    foreach(var attr in typeof(Retangle).GetCustomAttributes(false)){
        if(attr is DebugInfo dbi) Console.WriteLine(dbi);
    }
    // 
}

// inline
using System.Runtime.CompilerServices;

[MethodImpl(MethodImplOptions.AggressiveInlining)]
void InlineFunc(){}

// call constructor
public Point(): this(0, 0){}

// operator override
public static Point operator-(in Point p)=> new Point(-p.X, -p.Y);
public static Point operator*(in Point p, int s)=> new Point(p.X* s, p.Y* s);

```



```c#
// file storage
// config
var config = ConfigFile();
if(config.Load(SAVE_FILE)== Error.OK){
    HighScore = (int)config.GetValue("data", "highscore", 0);
    
    config.SetValue("data", "highscore", 100);
    config.Save(SAVE_FILE);
}

// json
var data = new Dictionary<string, object>(){ ["highscore"]= HighScore, };
var fout = new File();
fout.Open(SAVE_FILE, File.ModeFlags.Write);
fout.StoreLine(JSON.Print(data));
fout.Close();

var fin = new File();
fin.Open(SAVE_FILE, File.ModeFlags.Read);
string line = fin.GetLine();
var dict = JSON.Parse(line).Result as Dictionary;
HighScore = Convert.ToInt32(dict["highscore"]);
```



**thirdparty**

日志	`Serilog`

Linq加强	`MoreLinq`