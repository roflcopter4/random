package main


func quick_sort(data []tagdata, length int64) {
	do_sort(data, 0, length-2)
}


func do_sort(data []tagdata, first, last int64) {
	if first < last {
		pivot := partition(data, first, last)
		do_sort(data, first, pivot - 1)
		do_sort(data, pivot + 1, last)
	}
}


func partition(data []tagdata, first, last int64) int64 {
	var (
		p_val tagdata = data[first]
		left int64  = first + 1
		right int64 = last
		done bool   = false
	)

	for !done {
		// for left <= right && data[left] <= p_val {
		for left <= right && cmp(&data[left], &p_val) {
			left++
		}
		for left <= right && cmp(&p_val, &data[right]) {
			right--
		}

		if left > right {
			done = true
		} else {
			tmp := data[left]
			data[left] = data[right]
			data[right] = tmp
		}
	}

	tmp := data[first]
	data[first] = data[right]
	data[right] = tmp

	return right
}


func cmp(A, B *tagdata) (ret bool) {
	if A.kind == B.kind {
		if len(A.s) == len(B.s) {
			ret = A.s <= B.s
		} else {
			ret = len(A.s) <= len(B.s)
		}
	} else {
		ret = A.kind <= B.kind
	}
	return
}
