defmodule ExJieba.MPSegment do
  @on_load :init

  def init do
    load_nif()
  end

  defp load_nif do
    priv_path = :code.priv_dir(:exjieba)
    path = Path.join(priv_path, "mp_segment")
    :erlang.load_nif(path, 0)
    dict_path = [priv_path, "libcppjieba/dict/jieba.dict.utf8"] |> Path.join() |> to_charlist()
    load_dict(dict_path)
  end

  defp load_dict(_) do
    "NIF NOT LOADED"
  end

  def cut(_) do
    "NIF NOT LOADED"
  end
end
