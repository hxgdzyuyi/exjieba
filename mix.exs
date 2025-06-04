defmodule Mix.Tasks.Compile.Segment do
  def run(_) do
    Mix.shell().cmd("make segment")
    :ok
  end
end

defmodule Exjieba.Mixfile do
  use Mix.Project

  def project do
    [
      app: :exjieba,
      version: "0.0.3",
      elixir: "~> 1.17",
      compilers: [:segment, :elixir, :app],
      deps: deps()
    ]
  end

  def application do
    [mod: {ExJieba, []}]
  end

  defp deps do
    []
  end
end
