defmodule ExJiebaTest do
  @text "小明硕士毕业于中国科学院计算所，后在日本京都大学深造"
  use ExUnit.Case

  test "MPSegment" do
    assert ExJieba.MPSegment.cut(@text) == [
             "小",
             "明",
             "硕士",
             "毕业",
             "于",
             "中国科学院",
             "计算所",
             "，",
             "后",
             "在",
             "日本京都大学",
             "深造"
           ]
  end

  test "HMMSegment" do
    assert ExJieba.HMMSegment.cut(@text) == [
             "小明",
             "硕士",
             "毕业于",
             "中国",
             "科学院",
             "计算所",
             "，",
             "后",
             "在",
             "日",
             "本",
             "京",
             "都",
             "大",
             "学",
             "深",
             "造"
           ]
  end

  test "MixSegment" do
    assert ExJieba.MixSegment.cut(@text) == [
             "小明",
             "硕士",
             "毕业",
             "于",
             "中国科学院",
             "计算所",
             "，",
             "后",
             "在",
             "日本京都大学",
             "深造"
           ]
  end

  test "QuerySegment" do
    assert ExJieba.QuerySegment.cut(@text) == [
             "小明",
             "硕士",
             "毕业",
             "于",
             "中国",
             "中国科学院",
             "科学",
             "科学院",
             "学院",
             "计算所",
             "，",
             "后",
             "在",
             "日本",
             "日本京都大学",
             "京都",
             "京都大学",
             "大学",
             "深造"
           ]
  end

  test "Jieba default" do
    assert ExJieba.Jieba.cut("他来到了网易杭研大厦") == [
             "他",
             "来到",
             "了",
             "网易",
             "杭研",
             "大厦"
           ]

    assert ExJieba.Jieba.cut("我来自北京邮电大学。") == [
             "我",
             "来自",
             "北京邮电大学",
             "。"
           ]

    assert ExJieba.Jieba.cut_small("南京市长江大桥", 3) == [
             "南京市",
             "长江",
             "大桥"
           ]

    assert ExJieba.Jieba.cut_hmm("我来自北京邮电大学。。。学号123456") == [
             "我来",
             "自北京",
             "邮电大学",
             "。",
             "。",
             "。",
             "学号",
             "123456"
           ]

    assert ExJieba.Jieba.cut("我来自北京邮电大学。。。学号123456，用AK47") == [
             "我",
             "来自",
             "北京邮电大学",
             "。",
             "。",
             "。",
             "学号",
             "123456",
             "，",
             "用",
             "AK47"
           ]

    assert ExJieba.Jieba.cut_all("我来自北京邮电大学") == [
             "我",
             "来自",
             "北京",
             "北京邮电",
             "北京邮电大学",
             "邮电",
             "邮电大学",
             "电大",
             "大学"
           ]

    assert ExJieba.Jieba.cut_for_search("他来到了网易杭研大厦") == [
             "他",
             "来到",
             "了",
             "网易",
             "杭研",
             "大厦"
           ]
  end

  test "Jieba insert user word" do
    assert ExJieba.Jieba.cut("男默女泪") == ["男默", "女泪"]

    assert ExJieba.Jieba.insert_user_word("男默女泪") == :true

    assert ExJieba.Jieba.cut("男默女泪") == ["男默女泪"]

    assert ExJieba.Jieba.insert_user_word("同一个世界，同一个梦想") == :true

    assert Enum.join(ExJieba.Jieba.cut("同一个世界，同一个梦想"), "/") ==
             "同一个/世界/，/同一个/梦想"

    ExJieba.Jieba.reset_separators("")

    assert Enum.join(ExJieba.Jieba.cut("同一个世界，同一个梦想"), "/") ==
             "同一个世界，同一个梦想"
  end
end
