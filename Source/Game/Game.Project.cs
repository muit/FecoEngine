using System.IO; // for Path.Combine
using Sharpmake; // contains the entire Sharpmake object library.

[Generate]
class GameProject : BaseProject
{
    public GameProject() : base("Game") {}

    public override void ConfigureAll(Configuration conf, RiftTarget target)
    {
        base.ConfigureAll(conf, target);
        conf.SolutionFolder = "Game";

        conf.IncludePaths.Add(@"[project.SharpmakeCsPath]");
    
        conf.Options.Add(Sharpmake.Options.Vc.Linker.SubSystem.Application);

        conf.AddPublicDependency<EngineModule>(target);
    }
}
