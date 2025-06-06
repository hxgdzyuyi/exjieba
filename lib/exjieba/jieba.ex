defmodule ExJieba.Jieba do
  @on_load :init

  def init do
    load_nif()
  end

  def load_nif do
    priv_path = :code.priv_dir(:exjieba)
    path = Path.join(priv_path, "jieba")
    :erlang.load_nif(path, 0)
    dict_path = [priv_path, "libcppjieba/dict/jieba.dict.utf8"] |> Path.join() |> to_charlist()
    model_path = [priv_path, "libcppjieba/dict/hmm_model.utf8"] |> Path.join() |> to_charlist()
    user_dict_path = [priv_path, "libcppjieba/dict/user.dict.utf8"] |> Path.join() |> to_charlist()
    idf_path = [priv_path, "libcppjieba/dict/idf.utf8"] |> Path.join() |> to_charlist()
    stop_word_path = [priv_path, "libcppjieba/dict/stop_words.utf8"] |> Path.join() |> to_charlist()
    load_dict(dict_path, model_path, user_dict_path, idf_path, stop_word_path)
  end

  defp load_dict(_, _, _, _, _) do
    "NIF NOT LOADED"
  end

  def cut(_) do
    "NIF NOT LOADED"
  end

  def cut_all(_) do
    "NIF NOT LOADED"
  end

  def cut_for_search(_) do
    "NIF NOT LOADED"
  end

  def cut_hmm(_) do
    "NIF NOT LOADED"
  end

  def cut_small(_, _) do
    "NIF NOT LOADED"
  end

  def insert_user_word(_) do
    "NIF NOT LOADED"
  end

  def reset_separators(_) do
    "NIF NOT LOADED"
  end

  def tag(_) do
    "NIF NOT LOADED"
  end

  def lookup_tag(_) do
    "NIF NOT LOADED"
  end

  def delete_user_word(_) do
    "NIF NOT LOADED"
  end

  def find(_) do
    "NIF NOT LOADED"
  end
end
