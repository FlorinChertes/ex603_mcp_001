

template <typename... Ts>
auto add(Ts... args)
{
	return (... + args);
}

template <typename... Ts>
auto add_to_one(Ts... args)
{
	return (1 + ... + args);
}

